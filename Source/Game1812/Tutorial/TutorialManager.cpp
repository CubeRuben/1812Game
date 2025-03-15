#include "TutorialManager.h"

#include "Steps/TutorialBaseStep.h"
#include "../UI/TutorialWidget.h"
#include "../Pawns/Player/PlayerPawn.h"

#include <UMG.h>

ATutorialManager::ATutorialManager()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentStepIndex = 0;
	TimerDelay = 4.0f;
}

void ATutorialManager::BeginPlay()
{
	Super::BeginPlay();

	if (TutorialSteps.IsEmpty())
		return;

	if (TutorialWidgetClass)
	{
		TutorialWidget = CreateWidget<UTutorialWidget>(GetWorld(), TutorialWidgetClass.Get());

		if (!TutorialWidget)
			return;

		TutorialWidget->AddToViewport();
		TutorialWidget->InitManager(this);
	}

	BeginStep(0);
}

void ATutorialManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATutorialManager::BeginStep(int StepIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	if (StepIndex != CurrentStepIndex)
		TutorialSteps[CurrentStepIndex]->StepEnd();

	if (!TutorialSteps.IsValidIndex(StepIndex)) 
	{
		TutorialWidget->RemoveFromParent();
		Destroy();
		return;
	}

	CurrentStepIndex = StepIndex;
	DialogsIndex = 0;
	TipsIndex = 0;

	TutorialSteps[CurrentStepIndex]->StepStart(this);

	NextDialog();
}

void ATutorialManager::NextStep()
{
	BeginStep(CurrentStepIndex + 1);
}

void ATutorialManager::ForceFinish()
{
	if (TutorialSteps.IsValidIndex(CurrentStepIndex))
		TutorialSteps[CurrentStepIndex]->StepEnd();

	if (TutorialWidget)
		TutorialWidget->RemoveFromParent();

	Destroy();
}

void ATutorialManager::NextDialog()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	const UTutorialBaseStep* currentStep = TutorialSteps[CurrentStepIndex];

	if (!currentStep->GetDialogs().IsValidIndex(DialogsIndex))
	{
		NextTip();
		return;
	}

	const FText dialogText = currentStep->GetDialogs()[DialogsIndex];
	TutorialWidget->ChangeDialogText(dialogText);

	DialogsIndex++;

	if (currentStep->GetDialogs().IsValidIndex(DialogsIndex))
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATutorialManager::NextDialog, TimerDelay, false);
	}
	else 
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATutorialManager::NextTip, TimerDelay, false);
	}
}

void ATutorialManager::NextTip()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	const UTutorialBaseStep* currentStep = TutorialSteps[CurrentStepIndex];

	if (!currentStep->GetTips().IsValidIndex(TipsIndex))
	{
		NextTip();
		return;
	}

	const FText tipText = currentStep->GetTips()[TipsIndex];
	TutorialWidget->ChangeTipText(tipText);

	TipsIndex++;

	if (currentStep->GetTips().IsValidIndex(TipsIndex))
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATutorialManager::NextTip, TimerDelay, false);
	}
}

APlayerPawn* ATutorialManager::GetPlayerPawn()
{
	if (PlayerPawn.IsValid())
		return PlayerPawn.Get();

	APlayerController* controller = GetWorld()->GetFirstPlayerController();

	if (!controller)
		return nullptr;

	PlayerPawn = Cast<APlayerPawn>(controller->GetPawn());

	if (PlayerPawn.IsValid())
		return PlayerPawn.Get();

	return nullptr;
}