#include "StepCombatUnitsMovement.h"

#include "../TutorialManager.h"
#include "../../CossacksGameState.h"
#include "../../Pawns/Unit/Units/CombatUnit.h"
#include "../../Pawns/Unit/Components/UnitMovementComponent.h"

UStepCombatUnitsMovement::UStepCombatUnitsMovement()
{

}

void UStepCombatUnitsMovement::OnUnitMovementEnd(ABaseUnit* Unit)
{
	ACombatUnit* combatUnit = Cast<ACombatUnit>(Unit);

	if (!combatUnit)
		return;

	CombatUnits.Remove(combatUnit);

	if (CombatUnits.IsEmpty())
		Manager->NextStep();
}

void UStepCombatUnitsMovement::StepStart()
{
	ACossacksGameState* gameState = GetWorld()->GetGameState<ACossacksGameState>();
	
	if (!gameState)
		return;

	CombatUnits.Empty();
	gameState->GetCombatUnitsByTeam(CombatUnits, ETeam::Russia);

	DelegateHandle = UUnitMovementComponent::AddOnMovementEndGlobalHandler(FOnMovementEndStaticDelegate::FDelegate::CreateUObject(this, &UStepCombatUnitsMovement::OnUnitMovementEnd));
}

void UStepCombatUnitsMovement::StepEnd()
{
	UUnitMovementComponent::RemoveOnMovementEndGlobalHandler(DelegateHandle);
}
