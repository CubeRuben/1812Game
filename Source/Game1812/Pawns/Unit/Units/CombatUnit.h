#pragma once

#include "CoreMinimal.h"
#include "../BaseUnit.h"
#include "../Components/Damageable.h"
#include "CombatUnit.generated.h"

UCLASS()
class GAME1812_API ACombatUnit : public ABaseUnit, public IDamageable
{
	GENERATED_BODY()

public:

	ACombatUnit();

protected:

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UUnitCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UUnitReportComponent* ReportComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UUnitPieceProjectionComponent* PieceProjectionComponent;
	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCombatUnitOrder* CurrentOrder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCombatUnitDataAsset* CombatUnitData;

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:

	virtual void Tick(float DeltaTime) override;
	virtual void SpawnDefaultController() override;

	class UUnitCombatComponent* GetCombatComponent() const { return CombatComponent; };

	class UUnitReportComponent* GetReportComponent() const { return ReportComponent; };

	class UCombatUnitOrder* GetCombatUnitOrder() const { return CurrentOrder; };

	struct FCombatUnitStats* GetCombatUnitStats() const;

	void SetCombatUnitData(class UCombatUnitDataAsset* NewCombatUnitData);
	const UCombatUnitDataAsset* GetCombatUnitData() const { return CombatUnitData; };

	UFUNCTION(BlueprintImplementableEvent)
	void OnCombatUnitDataChange();

	bool IsInReachToReorganize(ACombatUnit* OtherUnit);

	//ABaseUnit class override
	class UUnitMovementComponent* GetMovementComponent() override;

	float GetMovementSpeed() const override;
	float GetRotationSpeed() const override;

	class UUnitOrder* GetCurrentOrder();
	void AssignOrder(class UUnitOrder* NewOrder);
	//
	
	//IDamageable Interface
	float ApplyDamage(IDamageable* Attacker, float Amount) override;
	ETeam GetTeam() const override { return Team; };
	ECombatUnitType GetUnitType() const override;
	FVector GetLocation() override;
	bool IsValidTarget() override;
	float GetDefense() override;
	//
};
