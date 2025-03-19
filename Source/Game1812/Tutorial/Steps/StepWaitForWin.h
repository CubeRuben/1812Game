#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepWaitForWin.generated.h"

UCLASS()
class GAME1812_API UStepWaitForWin : public UTutorialBaseStep
{
	GENERATED_BODY()
	
public:

	UStepWaitForWin();

protected:

	UFUNCTION()
	void OnWin();
	
	UFUNCTION()
	void OnLost();

public:
	
	void StepStart() override;
	void StepEnd() override;

};
