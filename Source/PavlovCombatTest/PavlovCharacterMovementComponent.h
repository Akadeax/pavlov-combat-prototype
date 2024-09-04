#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PavlovCharacterMovementComponent.generated.h"

UCLASS()

class PAVLOVCOMBATTEST_API UPavlovCharacterMovementComponent : public UCharacterMovementComponent
{
private:
	GENERATED_BODY()
#pragma region Networking shit
	class FSavedMove_Pavlov : public FSavedMove_Character
	{
		uint8 bWantsToSprint:1;
	
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override; 
	};
	class FNetworkPredictionData_Client_Pavlov : public FNetworkPredictionData_Client_Character
	{
		public:
		FNetworkPredictionData_Client_Pavlov(const UCharacterMovementComponent& ClientMovement);
		virtual FSavedMovePtr AllocateNewMove() override;
	};
#pragma endregion

	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))bool bWantsToSprint;

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;
public:
	UPavlovCharacterMovementComponent();
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UFUNCTION(BluePrintCallable) void SprintPressed();
	UFUNCTION(BluePrintCallable) void SprintReleased();
	
	
protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	
};
