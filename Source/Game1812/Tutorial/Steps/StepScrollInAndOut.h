#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepScrollInAndOut.generated.h"

UCLASS()
class GAME1812_API UStepScrollInAndOut : public UTutorialBaseStep
{
	GENERATED_BODY()

public:

	UStepScrollInAndOut();

protected:

	bool bScrolledIn;

	void ScrolledIn();
	void ScrolledOut();

public:

	void StepStart(class ATutorialManager* TutorialManager) override;
	void StepEnd() override;
	
};
