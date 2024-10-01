#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssignedUnitOrder.generated.h"

USTRUCT(BlueprintType)
struct GAME1812_API FAssignedCombatUnitOrder
{
	GENERATED_BODY();

public:

	FAssignedCombatUnitOrder();
	FAssignedCombatUnitOrder(const FAssignedCombatUnitOrder& Other);
	FAssignedCombatUnitOrder(class UCombatUnitOrder* UnitOrder, class ABaseUnit* Unit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCombatUnitOrder* UnitOrder;

	TWeakObjectPtr<class ABaseUnit> Unit;

	bool operator==(const FAssignedCombatUnitOrder& Other);
};