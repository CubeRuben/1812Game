#include "StepLookAtMap.h"

#include "../TutorialManager.h"
#include "../../Pawns/Player/PlayerPawn.h"
#include "../../Pawns/Player/Components/PlayerMovementComponent.h"

UStepLookAtMap::UStepLookAtMap()
{

}

void UStepLookAtMap::LookedAtMap()
{
	Manager->NextStep();
}

void UStepLookAtMap::StepStart(ATutorialManager* TutorialManager)
{
	Super::StepStart(TutorialManager);

	Manager->GetPlayerPawn()->GetMovementComponent()->OnLookAtMap.BindUObject(this, &UStepLookAtMap::LookedAtMap);
}

void UStepLookAtMap::StepEnd()
{
	Manager->GetPlayerPawn()->GetMovementComponent()->OnLookAtMap.Unbind();
}
