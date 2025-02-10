#include "StepScrollInAndOut.h"

#include "../TutorialManager.h"
#include "../../Pawns/Player/PlayerPawn.h"
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

void UStepScrollInAndOut::StepStart(ATutorialManager* TutorialManager)
{
	Super::StepStart(TutorialManager);

	Manager->GetPlayerPawn()->GetCameraArmComponent()->OnFullScrollIn.BindUObject(this, &UStepScrollInAndOut::ScrolledIn);
	Manager->GetPlayerPawn()->GetCameraArmComponent()->OnFullScrollOut.BindUObject(this, &UStepScrollInAndOut::ScrolledOut);
}

void UStepScrollInAndOut::StepEnd()
{
	Manager->GetPlayerPawn()->GetCameraArmComponent()->OnFullScrollIn.Unbind();
	Manager->GetPlayerPawn()->GetCameraArmComponent()->OnFullScrollOut.Unbind();
}
