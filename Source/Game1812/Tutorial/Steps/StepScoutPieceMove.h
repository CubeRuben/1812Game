#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepScoutPieceMove.generated.h"

UCLASS()
class GAME1812_API UStepScoutPieceMove : public UTutorialBaseStep
{
	GENERATED_BODY()
	
public:

	UStepScoutPieceMove();

protected:

	FDelegateHandle DelegateHandle;

	void PieceAddedToMap(class APiece* Piece);

	void SetPiecesOutlineEnabled(bool OutlineEnabled);

public:

	void StepStart() override;
	void StepEnd() override;
};
