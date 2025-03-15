#include "StepCombatPieceMove.h"

#include "../TutorialManager.h"
#include "../../Actors/Pieces/CombatPiece.h"

UStepCombatPieceMove::UStepCombatPieceMove()
{

}

void UStepCombatPieceMove::PieceMapHit(APiece* Piece)
{
	if (Cast<ACombatPiece>(Piece))
		Manager->NextStep();
}

void UStepCombatPieceMove::StepStart()
{
	APiece::OnMapHitWasDraggedGlobalEvent.BindUObject(this, &UStepCombatPieceMove::PieceMapHit);
}

void UStepCombatPieceMove::StepEnd()
{
	APiece::OnMapHitWasDraggedGlobalEvent.Unbind();
}
