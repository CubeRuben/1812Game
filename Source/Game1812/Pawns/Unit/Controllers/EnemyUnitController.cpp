#include "EnemyUnitController.h"

#include "../Units/CombatUnit.h"
#include "../Orders/UnitOrder.h"

TArray<AEnemyUnitController*> AEnemyUnitController::EnemyControllers = TArray<AEnemyUnitController*>();

AEnemyUnitController::AEnemyUnitController()
{
	CombatFormation = nullptr;
}

void AEnemyUnitController::BeginPlay()
{
	Super::BeginPlay();

	EnemyControllers.Add(this);
}

void AEnemyUnitController::BeginDestroy()
{
	Super::BeginDestroy();

	EnemyControllers.Remove(this);
}

void AEnemyUnitController::OnPossess(APawn* InPawn)
{
	if (!InPawn)
		return;

	CombatUnit = Cast<ACombatUnit>(InPawn);
}

void AEnemyUnitController::FindAlliesInRange(TArray<AEnemyUnitController*> Allies, float Range)
{
	for (AEnemyUnitController* controller : EnemyControllers) 
	{
		if (!controller)
			continue;
	}
}

ACombatUnit* AEnemyUnitController::GetCombatUnit() const
{
	return CombatUnit.Get();
}

