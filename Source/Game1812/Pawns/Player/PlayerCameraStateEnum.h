#pragma once

#include "CoreMinimal.h"
#include "PlayerCameraStateEnum.generated.h"

UENUM(BlueprintType)
enum class EPlayerCameraState : uint8
{
	OutOfMap = 0,
	MovingToMap = 1,
	LookingAtMap = 2
};