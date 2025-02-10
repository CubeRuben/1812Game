#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepGiveOrder.generated.h"

UCLASS()
class GAME1812_API UStepGiveOrder : public UTutorialBaseStep
{
	GENERATED_BODY()

public:

	UStepGiveOrder();

protected:

	void OrderAssigned(class APiece* Piece);

public:
	
	void StepStart(class ATutorialManager* TutorialManager) override;
	void StepEnd() override;
};
