#include "PavlovCharacterMovementComponent.h"



void UPavlovCharacterMovementComponent::SprintPressed()
{
	bWantsToSprint = true;
}

void UPavlovCharacterMovementComponent::SprintReleased()
{
	bWantsToSprint = false;
}


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
	
}
