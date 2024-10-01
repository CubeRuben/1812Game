#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CossacksGameInstance.generated.h"

UCLASS(BlueprintType)
class GAME1812_API UCossacksGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class APiece> CombatUnitPieceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class APiece> ScoutUnitPieceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AAdjutantUnit> AdjutantUnitClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AUnitPathArrow> UnitPathArrowClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AGhostPiece> GhostPieceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AEnemyPiece> EnemyPieceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInterface* PieceMapMarkerMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInterface* DeadPieceMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInterface* CombatStatsPieceMaterial;

public:

	UClass* GetCombatUnitPieceClass() const;
	UClass* GetScoutUnitPieceClass() const;

	UClass* GetAdjutantUnitClass() const;

	UClass* GetUnitPathArrowClass() const;
	UClass* GetGhostPieceClass() const;
	UClass* GetEnemyPieceClass() const;

	UMaterialInterface* GetPieceMapMarkerMaterial() const { return PieceMapMarkerMaterial; }
	UMaterialInterface* GetDeadPieceMaterial() const { return DeadPieceMaterial; };
	UMaterialInterface* GetCombatStatsPieceMaterial() const { return CombatStatsPieceMaterial; };

};
