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

UScoutUnitOrder::UScoutUnitOrder()
{
	
}

UAdjutantUnitOrder::UAdjutantUnitOrder()
{

}


