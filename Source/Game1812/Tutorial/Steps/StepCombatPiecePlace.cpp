#include "StepCombatPiecePlace.h"

#include "../TutorialManager.h"
#include "../TutorialCombatPiecesTrigger.h"
#include "../../Actors/Pieces/CombatPiece.h"

UStepCombatPiecePlace::UStepCombatPiecePlace() :
	RequiredNumberOfPieces(0)
{
}

void UStepCombatPiecePlace::PieceOverlapBegin(ACombatPiece* Piece)
{
	if (!Piece)
		return;

	OverlappedPieces.Add(Piece);

	if (RequiredNumberOfPieces <= OverlappedPieces.Num())
		Manager->NextStep();
}

void UStepCombatPiecePlace::PieceOverlapEnd(ACombatPiece* Piece)
{
	if (!Piece)
		return;

	OverlappedPieces.Remove(Piece);
}

void UStepCombatPiecePlace::StepStart()
{
	if (!TriggerActor.IsValid())
		return;

	TriggerActor->SetTriggerActive(true);

	OverlapBeginDelegateHandle = TriggerActor->AddOnPieceOverlapBeginHandler(FCombatPieceOverlapDelegate::FDelegate::CreateUObject(this, &UStepCombatPiecePlace::PieceOverlapBegin));
	OverlapEndDelegateHandle = TriggerActor->AddOnPieceOverlapEndHandler(FCombatPieceOverlapDelegate::FDelegate::CreateUObject(this, &UStepCombatPiecePlace::PieceOverlapEnd));
}

void UStepCombatPiecePlace::StepEnd()
{
	if (!TriggerActor.IsValid())
		return;

	TriggerActor->SetTriggerActive(false);

	TriggerActor->RemoveOnPieceOverlapBeginHandler(OverlapBeginDelegateHandle);
	TriggerActor->RemoveOnPieceOverlapEndHandler(OverlapEndDelegateHandle);
}
