#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PieceCombatStatsComponent.generated.h"

UCLASS()
class GAME1812_API UPieceCombatStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPieceCombatStatsComponent();

protected:
	
	class ACombatPiece* OwnerPiece;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class ACombatUnit> CombatUnit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMaterialInstanceDynamic* StatsMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentHPRatio;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentMoraleRatio;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float UnitHPRatio;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float UnitMoraleRatio;

	virtual void BeginPlay() override;

	void OnUnitSpawn();

	void UpdateUnitStats();

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool CanUpdateStats();
};
