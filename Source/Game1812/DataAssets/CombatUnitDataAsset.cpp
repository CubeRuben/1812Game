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

UCombatUnitDataAsset::UCombatUnitDataAsset() :
	UnitName("Unnamed Unit"), CombatUnitStats(FCombatUnitStats()), PieceMesh(nullptr), PieceFoundationMesh(nullptr), FormationSortPriority(0)
{
}

FCombatUnitVisual::FCombatUnitVisual() :
	UnitMesh(nullptr), HealthPointPerMesh(100.0f), MovementSpeed(0.0f), RotationSpeed(0.0f), ForceTeleportDistanceSquared(10000.0f), MeshOffset(10.0f)
{
}
