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

	void GetCombatUnitsByTeam(TArray<TObjectPtr<class ACombatUnit>>& OutArray, ETeam Team);

	const TArray<TObjectPtr<class ACombatUnit>>& GetCombatUnits() const { return CombatUnits; }
	const TArray<TObjectPtr<class AAdjutantUnit>>& GetAdjutantUnits() const { return AdjutantUnits; }
	const TArray<TObjectPtr<class AScoutUnit>>& GetScoutUnits() const { return ScoutUnits; }
	const TArray<TObjectPtr<class ACombatPiece>>& GetCombatPieces() const { return CombatPieces; }
	const TArray<TObjectPtr<class AScoutPiece>>& GetScoutPieces() const { return ScoutPieces; }

	float GetUnitsHP(ETeam Team);
};
