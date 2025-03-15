#include "StepGiveOrder.h"

#include "../TutorialManager.h"
#include "../../Actors/Pieces/Piece.h"

UStepGiveOrder::UStepGiveOrder()
{

}

void UStepGiveOrder::OrderAssigned(APiece* Piece)
{
	Manager->NextStep();
}

void UStepGiveOrder::StepStart()
{
	APiece::OnOrderAssignGlobalEvent.BindUObject(this, &UStepGiveOrder::OrderAssigned);
}

void UStepGiveOrder::StepEnd()
{
	APiece::OnOrderAssignGlobalEvent.Unbind();
}
