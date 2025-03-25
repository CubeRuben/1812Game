#include "StepOrdersSend.h"

#include "../TutorialManager.h"
#include "../../OrdersSenderComponent.h"
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

	UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

	if (!ordersSender)
		return;

	DelegateHandle = ordersSender->AddOnOrdersSendHandler(FOrdersSendDelegate::FDelegate::CreateUObject(this, &UStepOrdersSend::OnOrderSend));
}

void UStepOrdersSend::StepEnd()
{
	Manager->GetPlayerPawn()->GetInteractionComponent()->SetAllowedToInteract(true);

	UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

	if (!ordersSender)
		return;

	ordersSender->RemoveOnOrdersSendHandler(DelegateHandle);
}
