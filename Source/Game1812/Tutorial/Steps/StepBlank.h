#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepBlank.generated.h"

UCLASS()
class GAME1812_API UStepBlank : public UTutorialBaseStep
{
	GENERATED_BODY()

public:

	UStepBlank();

protected:


public:

	void StepStart() override;
	void StepEnd() override;
	
};
