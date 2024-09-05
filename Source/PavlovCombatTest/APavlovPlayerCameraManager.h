#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "APavlovPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PAVLOVCOMBATTEST_API APavlovPlayerCameraManager : public APlayerCameraManager
{
private:
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly) float CrouchBlendDuration = 0.25f;
	float CrouchBlendTime;

public:
	APavlovPlayerCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
