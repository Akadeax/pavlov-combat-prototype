#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PavlovCharacterMovementComponent.generated.h"

UCLASS()

class PAVLOVCOMBATTEST_API UPavlovCharacterMovementComponent : public UCharacterMovementComponent
{
private:
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))bool bWantsToSprint;

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;
public:
	UPavlovCharacterMovementComponent();

	UFUNCTION(BluePrintCallable) void SprintPressed();
	UFUNCTION(BluePrintCallable) void SprintReleased();
	
	
protected:

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	
};
