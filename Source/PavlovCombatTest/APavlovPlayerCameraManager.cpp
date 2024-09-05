// Fill out your copyright notice in the Description page of Project Settings.


#include "APavlovPlayerCameraManager.h"

#include "PavlovCharacter.h"
#include "PavlovCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

APavlovPlayerCameraManager::APavlovPlayerCameraManager()
	:APlayerCameraManager()
{
}

void APavlovPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);
	
	if (APavlovCharacter* PavlovCharacter = Cast<APavlovCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UPavlovCharacterMovementComponent* ZMC = PavlovCharacter->GetPavlovCharacterMovement();
		FVector TargetCrouchOffset = FVector(
			0,
			0,
			ZMC->GetCrouchedHalfHeight() - PavlovCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		);
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (ZMC->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		if (ZMC->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}
	}
}
