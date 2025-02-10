#include "CossacksGameState.h"

#include "Actors/Pieces/CombatPiece.h"
#include "Actors/Pieces/ScoutPiece.h"
#include "Pawns/Unit/Units/CombatUnit.h"
#include "Pawns/Unit/Units/AdjutantUnit.h"
#include "Pawns/Unit/Units/ScoutUnit.h"
#include "Pawns/Unit/Components/UnitCombatComponent.h"
#include "ObjectivesSystem/BattleObjectivesManager.h"
#include "DataAssets/CombatUnitDataAsset.h"

#include <Kismet/GameplayStatics.h>

#define ADD_CAST_ACTOR(Type, Array) if (Type* actor = Cast<Type>(Actor)) Array.Add(actor);

ACossacksGameState::ACossacksGameState()
{

}

void ACossacksGameState::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &ACossacksGameState::OnActorSpawn));

	IterateActors();

	if (ABattleObjectivesManager::GetInstance())
		ABattleObjectivesManager::GetInstance()->Init(this);
}

void ACossacksGameState::OnActorSpawn(AActor* Actor)
{
	if (!Actor)
		return;

	ADD_CAST_ACTOR(ACombatUnit, CombatUnits);
	ADD_CAST_ACTOR(AAdjutantUnit, AdjutantUnits);
	ADD_CAST_ACTOR(AScoutUnit, ScoutUnits);
	ADD_CAST_ACTOR(ACombatPiece, CombatPieces);
	ADD_CAST_ACTOR(AScoutPiece, ScoutPieces);
}

void ACossacksGameState::IterateActors()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actors);

	for (auto& actor : actors) 
	{
		OnActorSpawn(actor);
	}
}

void ACossacksGameState::GetCombatUnitsByTeam(TArray<TObjectPtr<class ACombatUnit>> OutArray, ETeam Team)
{
	OutArray.Empty();

	OutArray = CombatUnits.FilterByPredicate([Team](const TObjectPtr<class ACombatUnit>& el) { return !el ? false : (el.Get()->GetTeam() == Team); });
}

float ACossacksGameState::GetUnitsHP(ETeam Team)
{
	float hp = 0.0f;

	for (auto& unit : CombatUnits)
	{
		if (!unit)
			continue;

		if (unit->GetTeam() == Team) 
			hp += unit->GetCombatComponent()->GetHealthPoints();
	}

	if (Team == ETeam::France)
		return hp;

	for (auto& piece : CombatPieces) 
	{
		if (!piece)
			continue;

		if (piece->GetUnit())
			continue;

		hp += piece->GetCombatUnitData()->GetCombatUnitStats()->GetBaseHP();
	}

	return hp;
}
