#include "CombatUnitDataAsset.h"

ECombatUnitType FCombatUnitStats::GetUnitType() const
{
	return UnitType;
}

float FCombatUnitStats::GetMaxMovementSpeed() const
{
	return MaxMovementSpeed;
}

float FCombatUnitStats::GetMinMovementSpeed() const
{
	return MinMovementSpeed;
}

float FCombatUnitStats::GetRotationSpeed() const
{
	return RotationSpeed;
}

float FCombatUnitStats::GetBaseHP() const
{
	return BaseHP;
}

float FCombatUnitStats::GetAttackCooldown() const
{
	return AttackCooldown;
}

float FCombatUnitStats::GetBaseDamage() const
{
	return BaseDamage;
}

const TMap<ECombatUnitType, float>& FCombatUnitStats::GetDamageMultipliers() const
{
	return DamageMultipliers;
}

float FCombatUnitStats::GetBaseDefense() const
{
	return BaseDefense;
}

float FCombatUnitStats::GetMoraleRestorationSpeed() const
{
	return MoraleRestorationSpeed;
}

float FCombatUnitStats::GetMoraleLossDueToLosses() const
{
	return MoraleLossDueToLosses;
}

float FCombatUnitStats::GetDistanceForFullMoraleLoss() const
{
	return DistanceForFullMoraleLoss;
}

float FCombatUnitStats::GetAttackDistance() const
{
	return AttackDistance;
}

float FCombatUnitStats::GetEnemyDetectionRange() const
{
	return EnemyDetectionRange;
}

UCombatUnitDataAsset::UCombatUnitDataAsset()
{
	UnitName = "Unnamed Unit";
	CombatUnitStats = FCombatUnitStats();
	PieceMesh = nullptr;
	PieceFoundationMesh = nullptr;
}

FName UCombatUnitDataAsset::GetName() const
{
	return UnitName;
}

FCombatUnitStats* UCombatUnitDataAsset::GetCombatUnitStats()
{
	return &CombatUnitStats;
}

UStaticMesh* UCombatUnitDataAsset::GetPieceMesh() const
{
	return PieceMesh;
}

UStaticMesh* UCombatUnitDataAsset::GetPieceFoundationMesh() const
{
	return PieceFoundationMesh;
}

