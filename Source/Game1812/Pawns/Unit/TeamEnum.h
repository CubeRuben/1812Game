#pragma once

#include "CoreMinimal.h"
#include "TeamEnum.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8 {
	Russia = 0	UMETA(DisplayName = "Russian Empire"),
	France = 1	UMETA(DisplayName = "French Empire"),
};

