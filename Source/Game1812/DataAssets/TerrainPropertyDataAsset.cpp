#include "TerrainPropertyDataAsset.h"

UTerrainPropertyDataAsset::UTerrainPropertyDataAsset()
{
	
}

float UTerrainPropertyDataAsset::GetMovementSpeedModifier(ECombatUnitType CombatUnitType) const
{
	const float* value = MovementSpeedModifier.Find(CombatUnitType);

	if (value)
		return *value;

	return 1.f;
}

float UTerrainPropertyDataAsset::GetRotationSpeedModifier(ECombatUnitType CombatUnitType) const
{
	const float* value = RotationSpeedModifier.Find(CombatUnitType);

	if (value)
		return *value;

	return 1.f;
}

float UTerrainPropertyDataAsset::GetDamageModifier(ECombatUnitType CombatUnitType) const
{
	const float* value = DamageModifier.Find(CombatUnitType);

	if (value)
		return *value;

	return 1.f;
}

const TSubclassOf<UNavArea_CustomTerrain>& UTerrainPropertyDataAsset::GetTerrainNavAreaClass() const
{
	return TerrainNavAreaClass;
}
