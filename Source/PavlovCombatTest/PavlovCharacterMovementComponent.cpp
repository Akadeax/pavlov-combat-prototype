#include "PavlovCharacterMovementComponent.h"

#include <iostream>

#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UPavlovCharacterMovementComponent::InitializeComponent()
{
	UCharacterMovementComponent::InitializeComponent();
	PavlovCharacterOwner = Cast<APavlovCharacter>(GetOwner());
}

void UPavlovCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	UCharacterMovementComponent::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		PhysSlide(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
	}
}


bool UPavlovCharacterMovementComponent::IsMovingOnGround() const
{
	return UCharacterMovementComponent::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide);
}

bool UPavlovCharacterMovementComponent::CanCrouchInCurrentState() const
{
	return UCharacterMovementComponent::CanCrouchInCurrentState() && IsMovingOnGround();
}
void UPavlovCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (MovementMode == MOVE_Walking && bWantsToSlide && bWantsToSprint)
	{
		FHitResult PotentialSlideSurface;
		if (Velocity.SizeSquared() > pow(Slide_MinSpeed, 2) && GetSlideSurface(PotentialSlideSurface))
		{
			EnterSlide();
		}
	}

	if (IsCustomMovementMode(CMOVE_Slide) && !bWantsToSlide)
	{
		SetMovementMode(MOVE_Walking);
	}
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}


#pragma region input
void UPavlovCharacterMovementComponent::SprintPressed()
{
	bWantsToSprint = true;
}

void UPavlovCharacterMovementComponent::SprintReleased()
{
	bWantsToSprint = false;
}

void UPavlovCharacterMovementComponent::SlidePressed()
{
	bWantsToSlide = !bWantsToSlide;
	bWantsToCrouch = !bWantsToCrouch;
}


#pragma endregion

bool UPavlovCharacterMovementComponent::IsSprinting() const
{
	return bWantsToSprint;
}

bool UPavlovCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

#pragma region Slide
void UPavlovCharacterMovementComponent::EnterSlide()
{
	std::cout << "entered slide" << std::endl;
	Velocity += Velocity.GetSafeNormal2D() * Slide_EnterImpulse;
	SetMovementMode(MOVE_Custom, CMOVE_Slide);
}

void UPavlovCharacterMovementComponent::ExitSlide()
{
	std::cout << "exited slide" << std::endl;
	bWantsToSlide = false;
	bWantsToCrouch = false;
	bWantsToSprint = false;
	
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);
	SetMovementMode(MOVE_Walking);
}

void UPavlovCharacterMovementComponent::PhysSlide(float DeltaTime, int32 SlideNumber)
{
	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}
	
	RestorePreAdditiveRootMotionVelocity();

	FHitResult SurfaceHit;
	if (Velocity.SizeSquared() < pow(Slide_MinSpeed, 2))
	{
		std::cout << Velocity.Size() << "<" << Slide_MinSpeed  << std::endl;
		
		ExitSlide();
		StartNewPhysics(DeltaTime, SlideNumber);
		return;
	}

	// Surface Gravity
	Velocity += Slide_GravityForce * FVector::DownVector * DeltaTime;

	// Strafe
	if (FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal(), UpdatedComponent->GetRightVector())) > .5)
	{
		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());
	}
	else
	{
		Acceleration = FVector::ZeroVector;
	}

	// Calc Velocity
	if(!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		CalcVelocity(DeltaTime, Slide_Friction, true, GetMaxBrakingDeceleration());
	}
	ApplyRootMotionToVelocity(DeltaTime);

	SlideNumber++;
	bJustTeleported = false;
	
	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * DeltaTime;
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();
	SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);

	if (Hit.Time < 1.f)
	{
		HandleImpact(Hit, DeltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	FHitResult NewSurfaceHit;
	if (Velocity.SizeSquared() < pow(Slide_MinSpeed, 2))
	{
		ExitSlide();
	}
	
	// Update Outgoing Velocity & Acceleration
	if( !bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
	}
}

bool UPavlovCharacterMovementComponent::GetSlideSurface(FHitResult& OutHit) const
{
	FVector start = UpdatedComponent->GetComponentLocation();
	FVector end = start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*2.f*FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	return GetWorld()->LineTraceSingleByProfile(OutHit,start, end, ProfileName, PavlovCharacterOwner->GetIgnoreCharacterParams());
}
#pragma endregion


void UPavlovCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,const FVector& OldVelocity)
{
	UCharacterMovementComponent::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	if (MovementMode ==MOVE_Walking)
	{
		if (bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		} else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}


UPavlovCharacterMovementComponent::UPavlovCharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}
