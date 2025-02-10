#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepLookAtMap.generated.h"

UCLASS()
class GAME1812_API UStepLookAtMap : public UTutorialBaseStep
{
	GENERATED_BODY()

public:

	UStepLookAtMap();

protected:

	void LookedAtMap();

public:

	void StepStart(class ATutorialManager* TutorialManager) override;
	void StepEnd() override;
	
};
