#pragma once

#include "CoreMinimal.h"
#include "UnitTypeEnum.generated.h"

UENUM(BlueprintType)
enum class ECombatUnitType : uint8
{
	None		UMETA(DisplayName = "None"),
	Infantry	UMETA(DisplayName = "Infantry"),
	Cavalry		UMETA(DisplayName = "Cavalry"),
	Artillery	UMETA(DisplayName = "Artillery")
};

