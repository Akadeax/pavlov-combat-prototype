#include "PavlovCharacterMovementComponent.h"

#include "GameFramework/Character.h"

 #pragma region Networking shit
 bool UPavlovCharacterMovementComponent::FSavedMove_Pavlov::CanCombineWith(const FSavedMovePtr& NewMove,ACharacter* InCharacter, float MaxDelta) const
 {
 	FSavedMove_Pavlov* NewPavlovMovePtr = static_cast<FSavedMove_Pavlov*>(NewMove.Get());
 	if (bWantsToSprint != NewPavlovMovePtr->bWantsToSprint)
 	{
 		return false;
 	}
 	
 	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
 }

 void UPavlovCharacterMovementComponent::FSavedMove_Pavlov::Clear()
 {
 	FSavedMove_Character::Clear();
 	bWantsToSprint = false;
 }

 uint8 UPavlovCharacterMovementComponent::FSavedMove_Pavlov::GetCompressedFlags() const
 {
 	uint8 result = FSavedMove_Character::GetCompressedFlags();

 	if (bWantsToSprint) result |= FLAG_Custom_0;
 	
 	return result;
 }

 void UPavlovCharacterMovementComponent::FSavedMove_Pavlov::SetMoveFor(ACharacter* C, float InDeltaTime,FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
 {
 	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
 		
 	UPavlovCharacterMovementComponent* CharacterMovement = Cast<UPavlovCharacterMovementComponent>(C->GetCharacterMovement());
 	bWantsToSprint = CharacterMovement->bWantsToSprint;
 }

 void UPavlovCharacterMovementComponent::FSavedMove_Pavlov::PrepMoveFor(ACharacter* C)
 {
 	FSavedMove_Character::PrepMoveFor(C);
 	UPavlovCharacterMovementComponent* CharacterMovement = Cast<UPavlovCharacterMovementComponent>(C->GetCharacterMovement());

 	CharacterMovement->bWantsToSprint = bWantsToSprint;
 }

 UPavlovCharacterMovementComponent::FNetworkPredictionData_Client_Pavlov::FNetworkPredictionData_Client_Pavlov(const UCharacterMovementComponent& ClientMovement)
 	:FNetworkPredictionData_Client_Character(ClientMovement)
 {
 }

 FSavedMovePtr UPavlovCharacterMovementComponent::FNetworkPredictionData_Client_Pavlov::AllocateNewMove()
 {
 	return FSavedMovePtr(new FSavedMove_Pavlov());
 }

 FNetworkPredictionData_Client* UPavlovCharacterMovementComponent::GetPredictionData_Client() const
 {
 	check(PawnOwner != nullptr);

 	if (ClientPredictionData == nullptr)
 	{
 		UPavlovCharacterMovementComponent* MutableThis = const_cast<UPavlovCharacterMovementComponent*>(this);

 		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Pavlov(*this);
 		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist =92.f;
 		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
 	}
 	return ClientPredictionData; 
 }
void UPavlovCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	UCharacterMovementComponent::UpdateFromCompressedFlags(Flags);
	bWantsToSprint = (Flags & FSavedMove_Pavlov::FLAG_Custom_0) != 0;
}
 #pragma endregion


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
