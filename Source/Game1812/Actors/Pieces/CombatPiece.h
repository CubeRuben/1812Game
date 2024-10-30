#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "../../Pawns/Player/Components/InteractableSortable.h"
#include "CombatPiece.generated.h"

UCLASS()
class GAME1812_API ACombatPiece : public APiece, public IInteractableSortable
{
	GENERATED_BODY()
	
public:

	ACombatPiece();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPieceCombatStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPieceCombatOrderWidgetComponent* CombatOrderWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCombatUnitDataAsset* CombatUnitData;

	void SpawnUnit() override;

	void UpdatePieceMesh();

public:

	class UCombatUnitDataAsset* GetCombatUnitData() const;
	void SetCombatUnitData(class UCombatUnitDataAsset* NewCombatUnitData);

	virtual void AssignOrder(class UUnitOrder* UnitOrder) override;

	//
	// IInteractableSortable Interface
	//
	
	int GetPriority() const override;

	//
};
