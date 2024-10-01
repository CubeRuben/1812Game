#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Pawns/Unit/UnitTypeEnum.h"
#include "CombatUnitDataAsset.generated.h"

USTRUCT(BlueprintType)
struct GAME1812_API FCombatUnitStats
{
	GENERATED_USTRUCT_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECombatUnitType UnitType = ECombatUnitType::Infantry;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMovementSpeed = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinMovementSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseHP = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseDamage = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ECombatUnitType, float> DamageMultipliers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseDefense = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoraleRestorationSpeed = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoraleLossDueToLosses = 0.001f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DistanceForFullMoraleLoss = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackDistance = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EnemyDetectionRange = 200.0f;

public:

	ECombatUnitType GetUnitType() const;
	float GetMaxMovementSpeed() const;
	float GetMinMovementSpeed() const;
	float GetRotationSpeed() const;
	float GetBaseHP() const;
	float GetAttackCooldown() const;
	float GetBaseDamage() const;
	const TMap<ECombatUnitType, float>& GetDamageMultipliers() const;
	float GetBaseDefense() const;
	float GetMoraleRestorationSpeed() const;
	float GetMoraleLossDueToLosses() const;
	float GetDistanceForFullMoraleLoss() const;
	float GetAttackDistance() const;
	float GetEnemyDetectionRange() const;

};

UCLASS(BlueprintType)
class GAME1812_API UCombatUnitDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UCombatUnitDataAsset();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCombatUnitStats CombatUnitStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* PieceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* PieceFoundationMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* UnitMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform UnitMeshTransform;

public:

	FName GetName() const;
	FCombatUnitStats* GetCombatUnitStats();
	UStaticMesh* GetPieceMesh() const;
	UStaticMesh* GetPieceFoundationMesh() const;
	
};
