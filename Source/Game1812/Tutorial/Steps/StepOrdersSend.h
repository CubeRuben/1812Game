#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepOrdersSend.generated.h"

UCLASS()
class GAME1812_API UStepOrdersSend : public UTutorialBaseStep
{
	GENERATED_BODY()
	
public:

	UStepOrdersSend();

protected:

	FDelegateHandle DelegateHandle;

	void OnOrderSend();

public:

	void StepStart() override;
	void StepEnd() override;
};
