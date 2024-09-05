#pragma once

#include "CoreMinimal.h"
#include "PavlovCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PavlovCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None		UMETA(Hidden),
	CMOVE_Slide		UMETA(DisplayName="Slide"),
	CMOVE_MAX		UMETA(Hidden),
};

UCLASS()


class PAVLOVCOMBATTEST_API UPavlovCharacterMovementComponent : public UCharacterMovementComponent
{
private:
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed =750;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed = 500;

	UPROPERTY(EditDefaultsOnly) float Slide_MinSpeed=550;
	UPROPERTY(EditDefaultsOnly) float Slide_EnterImpulse=2000;
	UPROPERTY(EditDefaultsOnly) float Slide_GravityForce=10000;
	UPROPERTY(EditDefaultsOnly) float Slide_Friction = 1.3f;

	
	bool bWantsToSprint;
	bool bWantsToSlide;

	UPROPERTY(Transient) APavlovCharacter* PavlovCharacterOwner;

	#pragma region slide

	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float DeltaTime, int32 SlideNumber);
	bool GetSlideSurface(FHitResult& OutHit) const;
	
	#pragma endregion

	
protected:
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void InitializeComponent() override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;



public:
	UPavlovCharacterMovementComponent();

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	
	UFUNCTION(BluePrintCallable) void SprintPressed();
	UFUNCTION(BluePrintCallable) void SprintReleased();
	UFUNCTION(BlueprintCallable) void SlidePressed();
	UFUNCTION(BluePrintPure) bool IsSprinting() const;
	UFUNCTION(BluePrintPure) bool IsSlideing() const {return bWantsToSlide;}
	UFUNCTION(BluePrintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	
	
	
};
