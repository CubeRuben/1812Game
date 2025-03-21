#pragma once

#include "CoreMinimal.h"
#include "TutorialBaseStep.h"
#include "StepCombatPiecePlace.generated.h"

UCLASS()
class GAME1812_API UStepCombatPiecePlace : public UTutorialBaseStep
{
	GENERATED_BODY()
	
public:

	UStepCombatPiecePlace();

protected:

	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<class ATutorialCombatPiecesTrigger> TriggerActor;

	UPROPERTY(EditAnywhere)
	TSet<TWeakObjectPtr<class ACombatPiece>> OverlappedPieces;

	UPROPERTY(EditAnywhere)
	int RequiredNumberOfPieces;

	FDelegateHandle OverlapBeginDelegateHandle;
	FDelegateHandle OverlapEndDelegateHandle;

	void PieceOverlapBegin(class ACombatPiece* Piece);
	void PieceOverlapEnd(class ACombatPiece* Piece);

public:

	const TSet<TWeakObjectPtr<class ACombatPiece>>& GetOverlappedPieces() const { return OverlappedPieces; }

	void StepStart() override;
	void StepEnd() override;

};
