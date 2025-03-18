#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepCombatUnitsMovement.generated.h"

UCLASS()
class GAME1812_API UStepCombatUnitsMovement : public UTutorialBaseStep
{
	GENERATED_BODY()
	
public:

	UStepCombatUnitsMovement();

protected:

	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<class ACombatUnit>> CombatUnits;

	FDelegateHandle DelegateHandle;

	void OnUnitMovementEnd(class ABaseUnit* Unit);

public:

	void StepStart() override;
	void StepEnd() override;

};
