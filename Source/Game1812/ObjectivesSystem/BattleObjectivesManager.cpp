#include "BattleObjectivesManager.h"

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

	StartHP = 0.f;
	TakenCasualties = 0.f;
	EnemyStartHP = 0.f;
	EnemyTakenCasualties = 0.f;
}

void ABattleObjectivesManager::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;

	GetWorld()->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &ABattleObjectivesManager::OnActorSpawn));

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ABattleObjectivesManager::ScanForData));

}

void ABattleObjectivesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void ABattleObjectivesManager::OnActorSpawn(AActor* Actor)
{
	if (!Actor)
		return;

	ACombatUnit* combatUnit = Cast<ACombatUnit>(Actor);

	if (combatUnit)
		combatUnit->GetCombatComponent()->OnDamageTaken.AddDynamic(this, &ABattleObjectivesManager::OnUnitDamageTaken);
}

void ABattleObjectivesManager::ScanForData()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACombatUnit::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		ACombatUnit* unit = Cast<ACombatUnit>(actor);

		if (!unit)
			continue;

		switch (unit->GetTeam())
		{
		case ETeam::Russia:
			StartHP += unit->GetCombatUnitStats()->GetBaseHP();
			break;
		case ETeam::France:
			EnemyStartHP += unit->GetCombatUnitStats()->GetBaseHP();
			break;
		default:
			break;
		}

		unit->GetCombatComponent()->OnDamageTaken.AddDynamic(this, &ABattleObjectivesManager::OnUnitDamageTaken);
	}

	actors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACombatPiece::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		ACombatPiece* piece = Cast<ACombatPiece>(actor);

		if (!piece)
			continue;

		StartHP += piece->GetCombatUnitData()->GetCombatUnitStats()->GetBaseHP();
	}
}

void ABattleObjectivesManager::OnUnitDamageTaken(ACombatUnit* Unit, float TakenDamage)
{
	if (!Unit)
		return;

	switch (Unit->GetTeam())
	{
	case ETeam::Russia:
		TakenCasualties += TakenDamage;
		break;
	case ETeam::France:
		EnemyTakenCasualties += TakenDamage;
		break;
	default:
		break;
	}
}


float ABattleObjectivesManager::GetStartHP() const
{
	return StartHP;
}

float ABattleObjectivesManager::GetTakenCasualties() const
{
	return TakenCasualties;
}

float ABattleObjectivesManager::GetEnemyStartHP() const
{
	return EnemyStartHP;
}

float ABattleObjectivesManager::GetEnemyTakenCasualties() const
{
	return EnemyTakenCasualties;
}