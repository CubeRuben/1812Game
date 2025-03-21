#include "StepOrdersSend.h"

#include "../TutorialManager.h"
#include "../../Actors/HeadQuarters.h"
#include "../../Pawns/Player/PlayerPawn.h"
#include "../../Pawns/Player/Components/PlayerInteractionComponent.h"

UStepOrdersSend::UStepOrdersSend()
{
	
}

void UStepOrdersSend::OnOrderSend()
{
	Manager->NextStep();
}

void UStepOrdersSend::StepStart()
{
	Manager->GetPlayerPawn()->GetInteractionComponent()->SetAllowedToInteract(false);

	AHeadQuarters* const hq = AHeadQuarters::GetInstance();

	if (!hq)
		return;

	DelegateHandle = hq->AddOnOrdersSendHandler(FOrdersSendDelegate::FDelegate::CreateUObject(this, &UStepOrdersSend::OnOrderSend));
}

void UStepOrdersSend::StepEnd()
{
	Manager->GetPlayerPawn()->GetInteractionComponent()->SetAllowedToInteract(true);

	AHeadQuarters* const hq = AHeadQuarters::GetInstance();

	if (!hq)
		return;

	hq->RemoveOnOrdersSendHandler(DelegateHandle);
}
