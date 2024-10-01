#include "AssignedUnitOrder.h"

#include "../BaseUnit.h"
#include "UnitOrder.h" 

FAssignedCombatUnitOrder::FAssignedCombatUnitOrder()
{
	UnitOrder = nullptr;
	Unit = nullptr;
}

FAssignedCombatUnitOrder::FAssignedCombatUnitOrder(const FAssignedCombatUnitOrder& Other)
{
	UnitOrder = Other.UnitOrder;
	Unit = Other.Unit;
}

FAssignedCombatUnitOrder::FAssignedCombatUnitOrder(UCombatUnitOrder* UnitOrder, ABaseUnit* Unit)
{
	this->UnitOrder = UnitOrder;
	this->Unit = Unit;
}

bool FAssignedCombatUnitOrder::operator==(const FAssignedCombatUnitOrder& Other)
{
	return (Unit.Get() == Other.Unit.Get());
}