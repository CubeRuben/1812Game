#include "StepWaitForWin.h"

#include "../TutorialManager.h"
#include "../../ObjectivesSystem/BattleObjectivesManager.h"

UStepWaitForWin::UStepWaitForWin()
{

}

void UStepWaitForWin::OnWin()
{
	Manager->NextStep();
}

void UStepWaitForWin::OnLost()
{

}

void UStepWaitForWin::StepStart()
{
	ABattleObjectivesManager* const objectivesManager = ABattleObjectivesManager::GetInstance();

	if (!objectivesManager)
		return;

	objectivesManager->OnBattleWin.AddDynamic(this, &UStepWaitForWin::OnWin);
	objectivesManager->OnBattleLost.AddDynamic(this, &UStepWaitForWin::OnLost);
}

void UStepWaitForWin::StepEnd()
{

}
