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

	void PieceMapHit(class APiece* Piece);

public:

	void StepStart(class ATutorialManager* TutorialManager) override;
	void StepEnd() override;
};
