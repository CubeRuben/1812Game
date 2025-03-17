#include "StepScoutPieceMove.h"

#include "../TutorialManager.h"
#include "../../Actors/Pieces/ScoutPiece.h"
#include "../../Actors/Pieces/Components/PieceOutlineComponent.h"
#include "../../CossacksGameState.h"

UStepScoutPieceMove::UStepScoutPieceMove()
{

}

void UStepScoutPieceMove::PieceAddedToMap(APiece* Piece)
{
	if (Cast<AScoutPiece>(Piece))
		Manager->NextStep();
}

void UStepScoutPieceMove::SetPiecesOutlineEnabled(bool OutlineEnabled)
{
	ACossacksGameState* const gameState = GetWorld()->GetGameState<ACossacksGameState>();

	if (!gameState)
		return;

	const TArray<TObjectPtr<AScoutPiece>>& scoutPieces = gameState->GetScoutPieces();

	for (const TObjectPtr<AScoutPiece>& scoutPiece : scoutPieces) 
	{
		if (scoutPiece.IsNull())
			continue;

		scoutPiece->GetOutlineComponent()->SetAttentionEnabled(OutlineEnabled);
	}
}

void UStepScoutPieceMove::StepStart()
{
	SetPiecesOutlineEnabled(true); 

	DelegateHandle = APiece::AddOnAddedToMapGlobalHandler(FPieceGlobalEventDelegate::FDelegate::CreateUObject(this, &UStepScoutPieceMove::PieceAddedToMap));
}

void UStepScoutPieceMove::StepEnd()
{
	SetPiecesOutlineEnabled(false);

	APiece::RemoveOnAddedToMapGlobalHandler(DelegateHandle);
}
