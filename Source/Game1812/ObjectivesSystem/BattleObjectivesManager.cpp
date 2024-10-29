#include "BattleObjectivesManager.h"

#include "../CossacksGameState.h"

#include <Kismet/GameplayStatics.h>
#include "BattleCondition.h"
#include "../Actors/Pieces/CombatPiece.h"
#include "../Pawns/Unit/Units/CombatUnit.h"
#include "../DataAssets/CombatUnitDataAsset.h"
#include "../Pawns/Unit/Components/UnitCombatComponent.h"

ABattleObjectivesManager* ABattleObjectivesManager::Instance = nullptr;

ABattleObjectivesManager* ABattleObjectivesManager::GetInstance()
{
	return Instance;
}


ABattleObjectivesManager::ABattleObjectivesManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5f;

	BattleState = EBattleState::OnGoing;

	StartHP = 0.0f;
	TakenCasualties = 0.0f;
	EnemyStartHP = 0.0f;
	EnemyTakenCasualties = 0.0f;
}

void ABattleObjectivesManager::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;
}

void ABattleObjectivesManager::Init(ACossacksGameState* NewGameState)
{
	GameState = NewGameState;

	if (!GameState)
		return;

	StartHP = NewGameState->GetUnitsHP(ETeam::Russia);
	EnemyStartHP = NewGameState->GetUnitsHP(ETeam::France);
}

void ABattleObjectivesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TakenCasualties = StartHP - GameState->GetUnitsHP(ETeam::Russia);
	EnemyTakenCasualties = EnemyStartHP - GameState->GetUnitsHP(ETeam::France);

	if (WinCondition)
		WinCondition->UpdateCondition(this);
	
	if (LoseCondition)
		LoseCondition->UpdateCondition(this);

	if (BattleState/*Games*/ != EBattleState::OnGoing)
		return;

	if (LoseCondition && LoseCondition->GetState())
	{
		//GEngine->AddOnScreenDebugMessage(100, 15.f, FColor::Red, "Lose");
		OnBattleLost.Broadcast();
		BattleState = EBattleState::Lose;
	}
	else if (WinCondition && WinCondition->GetState())
	{
		//GEngine->AddOnScreenDebugMessage(100, 15.f, FColor::Red, "Win");
		OnBattleWin.Broadcast();
		BattleState = EBattleState::Win;
	}

}