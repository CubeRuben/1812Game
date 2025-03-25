#include "StepGiveOrder.h"

#include "StepCombatPiecePlace.h"
#include "../TutorialManager.h"
#include "../../OrdersSenderComponent.h"
#include "../../Actors/Pieces/Piece.h"
#include "../../Actors/Pieces/CombatPiece.h"
#include "../../Pawns/Player/PlayerPawn.h"
#include "../../Pawns/Player/Components/PlayerInteractionComponent.h"

UStepGiveOrder::UStepGiveOrder()
{

}

void UStepGiveOrder::OrderAssigned(APiece* Piece)
{
	ACombatPiece* const combatPiece = Cast<ACombatPiece>(Piece);

	if (!combatPiece)
		return;

	OverlappedPieces.Remove(combatPiece);

	if (OverlappedPieces.Num() == 0)
		Manager->NextStep();
}

void UStepGiveOrder::StepStart()
{
	if (PreviousStep.IsValid())
		OverlappedPieces = PreviousStep->GetOverlappedPieces();

	UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

	if (ordersSender)
		ordersSender->SetAllowedToSendOrders(false);

	Manager->GetPlayerPawn()->GetInteractionComponent()->SetAllowedToDrag(false);

	DelegateHandle = APiece::AddOnOrderAssignGlobalHandler(FPieceGlobalEventDelegate::FDelegate::CreateUObject(this, &UStepGiveOrder::OrderAssigned));
}

void UStepGiveOrder::StepEnd()
{
	UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

	if (ordersSender)
		ordersSender->SetAllowedToSendOrders(true);

	Manager->GetPlayerPawn()->GetInteractionComponent()->SetAllowedToDrag(true);

	APiece::RemoveOnOrderAssignGlobalHandler(DelegateHandle);
}
