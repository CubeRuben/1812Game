#include "BattleCondition.h"

#include "BattleObjectivesManager.h"
#include "Kismet/GameplayStatics.h"
#include "../CossacksGameMode.h"

bool UBattleCondition::Condition(ABattleObjectivesManager* BattleObjectivesManager)
{
	return false;
}

bool UBattleCondition::UpdateCondition(ABattleObjectivesManager* BattleObjectivesManager)
{
	const bool newState = Condition(BattleObjectivesManager);

	if (newState != bState) 
	{
		bState = newState;
		OnStateChange.Broadcast(bState);
	}

	return bState;
}

bool UBattleCondition::GetState()
{
	return bState;
}

UAndLogicBattleCondition::UAndLogicBattleCondition()
{
	UIText = FText::FromString("All of: ");
}

bool UAndLogicBattleCondition::Condition(ABattleObjectivesManager* BattleObjectivesManager)
{
	bool newState = true;

	for (UBattleCondition* condition : Conditions) 
	{
		if (!condition) 
		{
			newState = false;
			continue;
		}

		newState = newState && condition->UpdateCondition(BattleObjectivesManager);
	}

	return newState;
}

UOrLogicBattleCondition::UOrLogicBattleCondition()
{
	UIText = FText::FromString("One of: ");
}

bool UOrLogicBattleCondition::Condition(ABattleObjectivesManager* BattleObjectivesManager)
{
	bool newState = false;

	for (UBattleCondition* condition : Conditions)
	{
		if (!condition)
			continue;

		newState = newState || condition->UpdateCondition(BattleObjectivesManager);
	}

	return newState;
}

UTimerBattleCondition::UTimerBattleCondition()
{
	Time = 60.f;
	UIText = FText::FromString("Hold out for {0} minutes");
}

bool UTimerBattleCondition::Condition(ABattleObjectivesManager* BattleObjectivesManager)
{
	ACossacksGameMode* gameMode = Cast<ACossacksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	return gameMode->GetGameTimeMinutes() > Time;
}



UDefeatEnemyBattleCondition::UDefeatEnemyBattleCondition()
{
	EnemyCasualtyPercentage = 50.f;
	UIText = FText::FromString("Inflict {0}% losses on the enemy army");
}

bool UDefeatEnemyBattleCondition::Condition(ABattleObjectivesManager* BattleObjectivesManager)
{
	if (!BattleObjectivesManager)
		return false;

	const int hp = FMath::RoundHalfFromZero(BattleObjectivesManager->GetEnemyStartHP());

	if (hp == 0.f)
		return false;

	const int casualties = FMath::RoundHalfFromZero(BattleObjectivesManager->GetEnemyTakenCasualties());
	const float ratio = (float)casualties / (float)hp * 100.f;

	return ratio > EnemyCasualtyPercentage;
}



UCasualtiesBattleCondition::UCasualtiesBattleCondition()
{
	CasualtyPercentage = 50.f;
	UIText = FText::FromString("Lose {0}% of army");
}

bool UCasualtiesBattleCondition::Condition(ABattleObjectivesManager* BattleObjectivesManager)
{
	if (!BattleObjectivesManager)
		return false;

	const int hp = FMath::RoundHalfFromZero(BattleObjectivesManager->GetStartHP());

	if (hp == 0.f)
		return false;

	const int casualties = FMath::RoundHalfFromZero(BattleObjectivesManager->GetTakenCasualties());
	const float ratio = (float)casualties / (float)hp * 100.f;

	return ratio > CasualtyPercentage;
}

