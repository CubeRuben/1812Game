#include "StepOrdersSend.h"

#include "../TutorialManager.h"
#include "../../Actors/HeadQuarters.h"

UStepOrdersSend::UStepOrdersSend()
{
	
}

void UStepOrdersSend::OnOrderSend()
{
	Manager->NextStep();
}

void UStepOrdersSend::StepStart()
{
	AHeadQuarters* const hq = AHeadQuarters::GetInstance();

	if (!hq)
		return;

	DelegateHandle = hq->AddOnOrdersSendHandler(FOrdersSendDelegate::FDelegate::CreateUObject(this, &UStepOrdersSend::OnOrderSend));
}

void UStepOrdersSend::StepEnd()
{
	AHeadQuarters* const hq = AHeadQuarters::GetInstance();

	if (!hq)
		return;

	hq->RemoveOnOrdersSendHandler(DelegateHandle);
}
