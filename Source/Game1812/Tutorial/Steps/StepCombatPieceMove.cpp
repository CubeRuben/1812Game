#include "StepCombatPieceMove.h"

#include "../TutorialManager.h"
#include "../../Actors/Pieces/CombatPiece.h"
#include "../../Actors/Pieces/Components/PieceOutlineComponent.h"
#include "../../CossacksGameState.h"

UStepCombatPieceMove::UStepCombatPieceMove()
{

}

void UStepCombatPieceMove::PieceAddedToMap(APiece* Piece)
{
	if (Cast<ACombatPiece>(Piece))
		Manager->NextStep();
}

void UStepCombatPieceMove::SetPiecesOutlineEnabled(bool OutlineEnabled)
{
	ACossacksGameState* const gameState = GetWorld()->GetGameState<ACossacksGameState>();

	if (!gameState)
		return;

	const TArray<TObjectPtr<ACombatPiece>>& combatPieces = gameState->GetCombatPieces();

	for (const TObjectPtr<ACombatPiece>& combatPiece : combatPieces)
	{
		if (!combatPiece)
			continue;

		combatPiece->GetOutlineComponent()->SetAttentionEnabled(OutlineEnabled);
	}
}

void UStepCombatPieceMove::StepStart()
{
	SetPiecesOutlineEnabled(true);

	DelegateHandle = APiece::AddOnAddedToMapGlobalHandler(FPieceGlobalEventDelegate::FDelegate::CreateUObject(this, &UStepCombatPieceMove::PieceAddedToMap));
}

void UStepCombatPieceMove::StepEnd()
{
	SetPiecesOutlineEnabled(false);

	APiece::RemoveOnAddedToMapGlobalHandler(DelegateHandle);
}
