#include "StepScrollInAndOut.h"

#include "../TutorialManager.h"
#include "../../Pawns/Player/PlayerPawn.h"
#include "../../Pawns/Player/Components/PlayerInteractionComponent.h"
#include "../../Pawns/Player/Components/PlayerMovementComponent.h"
#include "../../Pawns/Player/Components/CameraArmComponent.h"

UStepScrollInAndOut::UStepScrollInAndOut()
{
	bScrolledIn = false;
}

void UStepScrollInAndOut::ScrolledIn()
{
	bScrolledIn = true;
}

void UStepScrollInAndOut::ScrolledOut()
{
	if (!bScrolledIn)
		return;

	Manager->NextStep();
}

void UStepScrollInAndOut::StepStart()
{
	Manager->GetPlayerPawn()->GetInteractionComponent()->SetAllowedToInteract(false);

	Manager->GetPlayerPawn()->GetCameraArmComponent()->OnTutorialScrollIn.BindUObject(this, &UStepScrollInAndOut::ScrolledIn);
	Manager->GetPlayerPawn()->GetCameraArmComponent()->OnTutorialScrollOut.BindUObject(this, &UStepScrollInAndOut::ScrolledOut);
}

void UStepScrollInAndOut::StepEnd()
{
	Manager->GetPlayerPawn()->GetInteractionComponent()->SetAllowedToInteract(true);

	Manager->GetPlayerPawn()->GetCameraArmComponent()->OnTutorialScrollIn.Unbind();
	Manager->GetPlayerPawn()->GetCameraArmComponent()->OnTutorialScrollOut.Unbind();
}
