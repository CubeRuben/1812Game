#include "StepLookAround.h"

#include "../TutorialManager.h"
#include "../../Pawns/Player/PlayerPawn.h"
#include "../../Pawns/Player/Components/PlayerMovementComponent.h"

UStepLookAround::UStepLookAround()
{
	bLookedLeft = false;
	bLookedRight = false;
}

void UStepLookAround::LookedLeft()
{
	bLookedLeft = true;

	CheckComplete();
}

void UStepLookAround::LookedRight()
{
	bLookedRight = true;

	CheckComplete();
}

void UStepLookAround::CheckComplete()
{
	if (bLookedLeft && bLookedRight)
		Manager->NextStep();
}

void UStepLookAround::StepStart(ATutorialManager* TutorialManager)
{
	Super::StepStart(TutorialManager);

	Manager->GetPlayerPawn()->GetMovementComponent()->OnLookLeft.BindUObject(this, &UStepLookAround::LookedLeft);
	Manager->GetPlayerPawn()->GetMovementComponent()->OnLookRight.BindUObject(this, &UStepLookAround::LookedRight);
}

void UStepLookAround::StepEnd()
{
	Manager->GetPlayerPawn()->GetMovementComponent()->OnLookLeft.Unbind();
	Manager->GetPlayerPawn()->GetMovementComponent()->OnLookRight.Unbind();
}
