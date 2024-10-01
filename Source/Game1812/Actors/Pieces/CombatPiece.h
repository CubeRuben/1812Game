#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "CombatPiece.generated.h"

UCLASS()
class GAME1812_API ACombatPiece : public APiece
{
	GENERATED_BODY()
	
public:

	ACombatPiece();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPieceCombatStatsComponent* StatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCombatUnitDataAsset* CombatUnitData;

	virtual void BeginPlay() override;

	void OnSpawnUnit() override;

	void UpdatePieceMesh();

public:

	class UCombatUnitDataAsset* GetCombatUnitData() const;
	void SetCombatUnitData(class UCombatUnitDataAsset* NewCombatUnitData);

	virtual void AssignOrder(class UUnitOrder* UnitOrder) override;
};
