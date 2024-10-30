#pragma once

#include "CoreMinimal.h"
#include "PieceOrderWidgetComponent.h"
#include "PieceCombatOrderWidgetComponent.generated.h"

UCLASS()
class GAME1812_API UPieceCombatOrderWidgetComponent : public UPieceOrderWidgetComponent
{
	GENERATED_BODY()

public:

	UPieceCombatOrderWidgetComponent();

protected:

	void DefaultOrder(class UCombatUnitOrder* CombatUnitOrder);
	void CombineOrder(class UCombatUnitOrder* CombatUnitOrder);
	void RedistributeOrder(class UCombatUnitOrder* CombatUnitOrder);

	void GetSelectedCombatUnits(TArray<class ACombatUnit*>& OutArray);

public:

	void AssignOrder(class UUnitOrder* UnitOrder) override;
	


};
