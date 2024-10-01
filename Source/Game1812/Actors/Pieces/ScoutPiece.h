#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "ScoutPiece.generated.h"

UCLASS()
class GAME1812_API AScoutPiece : public APiece
{
	GENERATED_BODY()
	
public:

	AScoutPiece();

protected:

	FOnPieceChangeDelegate OnOrderPointAdd;

public:

	virtual void AssignOrder(class UUnitOrder* UnitOrder) override;

	bool CanBeGrouped() override { return false; }

	UFUNCTION(BlueprintCallable)
	void OnOrderPointAdded();

	void AddOnOrderPointAddHandler(const FOnPieceChangeDelegate::FDelegate& Handler) { OnOrderPointAdd.Add(Handler); };
};
