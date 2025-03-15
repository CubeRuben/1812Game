#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TutorialBaseStep.generated.h"

UCLASS(Abstract, EditInlineNew, CollapseCategories, Blueprintable)
class GAME1812_API UTutorialBaseStep : public UObject
{
	GENERATED_BODY()

public:

	UTutorialBaseStep();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FText> Dialogs;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FText> Tips;

	class ATutorialManager* Manager;

public:

	const TArray<FText>& GetDialogs() const { return Dialogs; }
	const TArray<FText>& GetTips() const { return Tips; }

	void StepStart(class ATutorialManager* TutorialManager);
	virtual void StepStart();
	virtual void StepEnd();
};
