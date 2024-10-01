#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Pawns/Unit/UnitTypeEnum.h"
#include "../Actors/Tools/NavArea_CustomTerrain.h"
#include "TerrainPropertyDataAsset.generated.h"

UCLASS()
class GAME1812_API UTerrainPropertyDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UTerrainPropertyDataAsset();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ECombatUnitType, float> MovementSpeedModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ECombatUnitType, float> RotationSpeedModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ECombatUnitType, float> DamageModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UNavArea_CustomTerrain> TerrainNavAreaClass;

public:

	float GetMovementSpeedModifier(ECombatUnitType CombatUnitType) const;
	float GetRotationSpeedModifier(ECombatUnitType CombatUnitType) const;
	float GetDamageModifier(ECombatUnitType CombatUnitType) const;
	const TSubclassOf<UNavArea_CustomTerrain>& GetTerrainNavAreaClass() const;
};
