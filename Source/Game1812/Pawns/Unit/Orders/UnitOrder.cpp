#include "UnitOrder.h"

UUnitOrder::UUnitOrder()
{
	

}

UCombatUnitOrder::UCombatUnitOrder()
{
	UnitEnemyReaction = EUnitEnemyReaction::Attack;

	bForcedMarch = false;
	bConcentratedFire = false;

	ReorganizationType = EUnitReorganization::None;

	Location = FVector::ZeroVector;
	YawRotation = 0.f;
}

void UCombatUnitOrder::ClearReorganizationOrder()
{
	ReorganizationType = EUnitReorganization::None;
	UnitToReorganizeWith = nullptr;
}

UScoutUnitOrder::UScoutUnitOrder()
{
	
}

UAdjutantUnitOrder::UAdjutantUnitOrder()
{

}


