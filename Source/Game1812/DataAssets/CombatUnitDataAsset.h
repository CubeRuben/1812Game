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

USTRUCT(BlueprintType)
struct GAME1812_API FCombatUnitVisual
{
	GENERATED_USTRUCT_BODY()

public:

	FCombatUnitVisual();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* UnitMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealthPointPerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForceTeleportDistanceSquared;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MeshOffset;

public:

	UStaticMesh* GetUnitMesh() const { return UnitMesh; }
	float GetHealthPointsPerMesh() const { return HealthPointPerMesh; }
	float GetMovementSpeed() const { return MovementSpeed; }
	float GetRotationSpeed() const { return RotationSpeed; }
	float GetForceTeleportDistanceSquared() const { return ForceTeleportDistanceSquared; }
	float GetMeshOffset() const { return MeshOffset; }
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
	FCombatUnitVisual CombatUnitVisual;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int FormationSortPriority;

public:

	const FName& GetName() const { return UnitName; };
	const FCombatUnitStats& GetCombatUnitStats() const { return CombatUnitStats; };
	UStaticMesh* GetPieceMesh() const { return PieceMesh; };
	UStaticMesh* GetPieceFoundationMesh() const { return PieceFoundationMesh; };
	const FCombatUnitVisual& GetCombatUnitVisual() const { return CombatUnitVisual; }
	int GetFormationSortPriority() const { return FormationSortPriority; };
};
