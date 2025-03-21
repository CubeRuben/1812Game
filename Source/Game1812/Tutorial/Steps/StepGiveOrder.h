#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepGiveOrder.generated.h"

UCLASS()
class GAME1812_API UStepGiveOrder : public UTutorialBaseStep
{
	GENERATED_BODY()

public:

	UStepGiveOrder();

protected:

	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<class UStepCombatPiecePlace> PreviousStep; 

	UPROPERTY(EditAnywhere)
	TSet<TWeakObjectPtr<class ACombatPiece>> OverlappedPieces;

	FDelegateHandle DelegateHandle;

	void OrderAssigned(class APiece* Piece);

public:
	
	void StepStart() override;
	void StepEnd() override;
};
