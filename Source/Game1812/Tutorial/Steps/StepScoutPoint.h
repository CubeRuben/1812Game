#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepScoutPoint.generated.h"

UCLASS()
class GAME1812_API UStepScoutPoint : public UTutorialBaseStep
{
	GENERATED_BODY()
	
public:

	UStepScoutPoint();

protected:

	UPROPERTY(VisibleAnywhere)
	int PointsCounter;

	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<class ATutorialScoutDiscoverPoint>> PointsToDiscover;

	void OnPointDiscover();

public:

	void StepStart() override;
	void StepEnd() override;
};
