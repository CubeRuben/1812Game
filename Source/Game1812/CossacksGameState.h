#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Pawns/Unit/TeamEnum.h"
#include "CossacksGameState.generated.h"

UCLASS()
class GAME1812_API ACossacksGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	ACossacksGameState();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class ACombatUnit>> CombatUnits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class AAdjutantUnit>> AdjutantUnits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class AScoutUnit>> ScoutUnits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class ACombatPiece>> CombatPieces;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class AScoutPiece>> ScoutPieces;

	void BeginPlay() override;

	void OnActorSpawn(AActor* Actor);

	void IterateActors();

public:

	void GetCombatUnitsByTeam(TArray<TObjectPtr<class ACombatUnit>> OutArray, ETeam Team);

	float GetUnitsHP(ETeam Team);
};
