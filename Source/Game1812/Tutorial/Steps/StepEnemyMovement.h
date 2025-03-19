#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepEnemyMovement.generated.h"

UCLASS()
class GAME1812_API UStepEnemyMovement : public UTutorialBaseStep
{
	GENERATED_BODY()
	
public:

	UStepEnemyMovement();

protected:

	UPROPERTY(EditAnywhere)
	TArray<FVector> TargetPoints;

	UPROPERTY(EditAnywhere)
	TArray<FVector> RevealPoints;

	UPROPERTY(EditAnywhere)
	float RevealRadius;

	UPROPERTY(EditAnywhere)
	float TimerTimeout;

	FTimerHandle TimerHandle;

	void MoveEnemyUnits();
	void RevealArea();

	void TimerEnd();

public:

	void StepStart() override;
	void StepEnd() override;
};
