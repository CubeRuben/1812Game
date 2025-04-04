#include "StepEnemyMovement.h"

#include "../TutorialManager.h"
#include "../../CossacksGameState.h"
#include "../../FogSystem/FogOfWar.h"
#include "../../Pawns/Unit/Units/CombatUnit.h"
#include "../../Pawns/Unit/Components/UnitMovementComponent.h"
#include "../../Pawns/Unit/Orders/UnitOrder.h"
#include "../../Pawns/Player/PlayerPawn.h"
#include "../../Pawns/Player/Components/PlayerInteractionComponent.h"

#include "Kismet/KismetArrayLibrary.h"

UStepEnemyMovement::UStepEnemyMovement() :
	RevealRadius(5.0f), TimerTimeout(10.f)
{
	
}

void UStepEnemyMovement::MoveEnemyUnits()
{
	ACossacksGameState* gameState = GetWorld()->GetGameState<ACossacksGameState>();

	if (!gameState)
		return;

	TArray<TWeakObjectPtr<ACombatUnit>> enemyCombatUnits;
	gameState->GetCombatUnitsByTeam(enemyCombatUnits, ETeam::France);

	for (int i = 0; i < enemyCombatUnits.Num() && i < TargetPoints.Num(); i++)
	{
		ACombatUnit* const unit = enemyCombatUnits[i].Get();

		if (!unit)
			continue;

		const FVector targetLocation = TargetPoints[i];

		UCombatUnitOrder* order = Cast<UCombatUnitOrder>(unit->GetCurrentOrder());

		if (!order)
			continue;

		order->Location = targetLocation;

		unit->AssignOrder(order);
	}
}

void UStepEnemyMovement::RevealArea()
{
	AFogOfWar* const fogOfWar = AFogOfWar::GetInstance();

	if (!fogOfWar)
		return;

	TImageBuilder<FVector4f> revealArea;
	revealArea.SetDimensions(fogOfWar->GetDimensions());
	revealArea.Clear(FVector4f::Zero());

	for (FVector point : RevealPoints) 
	{
		ApplyCircularBrushToImage(revealArea, fogOfWar->LocationToIndex(point), RevealRadius, FVector4f::One());
	}

	fogOfWar->AddDiscoveredArea(revealArea);
}

void UStepEnemyMovement::TimerEnd()
{
	Manager->NextStep();
}

void UStepEnemyMovement::StepStart()
{
	Manager->GetPlayerPawn()->GetInteractionComponent()->SetAllowedToInteract(false);

	MoveEnemyUnits();
	RevealArea();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UStepEnemyMovement::TimerEnd), TimerTimeout, false);
}

void UStepEnemyMovement::StepEnd()
{
	Manager->GetPlayerPawn()->GetInteractionComponent()->SetAllowedToInteract(true);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}
