#pragma once

#include "PavlovCombatTest.h"
#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "PavlovCharacter.generated.h"

UCLASS(config=Game)
class APavlovCharacter : public ACharacter
{
	GENERATED_BODY()

	// Components
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Movement) class UPavlovCharacterMovementComponent* PavlovCharacterMovementComponent;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input) float TurnRateGamepad;
	explicit APavlovCharacter(const FObjectInitializer& ObjectInitializer);
	FORCEINLINE UPavlovCharacterMovementComponent* GetPavlovCharacterMovement() const { return PavlovCharacterMovementComponent; }
	// Input
	FCollisionQueryParams GetIgnoreCharacterParams() const;
private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
