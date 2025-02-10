#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepCombatPieceMove.generated.h"

UCLASS()
class GAME1812_API UStepCombatPieceMove : public UTutorialBaseStep
{
	GENERATED_BODY()
	
public:

	UStepCombatPieceMove();

protected:

	void PieceMapHit(class APiece* Piece);

public:

	void StepStart(class ATutorialManager* TutorialManager) override;
	void StepEnd() override;
};
