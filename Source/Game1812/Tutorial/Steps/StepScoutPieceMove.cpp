#include "StepScoutPieceMove.h"

#include "../TutorialManager.h"
#include "../../Actors/Pieces/ScoutPiece.h"

UStepScoutPieceMove::UStepScoutPieceMove()
{

}

void UStepScoutPieceMove::PieceMapHit(APiece* Piece)
{
	if (Cast<AScoutPiece>(Piece))
		Manager->NextStep();
}

void UStepScoutPieceMove::StepStart(ATutorialManager* TutorialManager)
{
	Super::StepStart(TutorialManager);

	APiece::OnMapHitWasDraggedGlobalEvent.BindUObject(this, &UStepScoutPieceMove::PieceMapHit);
}

void UStepScoutPieceMove::StepEnd()
{
	APiece::OnMapHitWasDraggedGlobalEvent.Unbind();
}
