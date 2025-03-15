#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepLookAround.generated.h"

UCLASS()
class GAME1812_API UStepLookAround : public UTutorialBaseStep
{
	GENERATED_BODY()
	
public:

	UStepLookAround();

protected:

	bool bLookedLeft;
	bool bLookedRight;

	void LookedLeft();
	void LookedRight();

	void CheckComplete();

public:

	void StepStart() override;
	void StepEnd() override;
};
