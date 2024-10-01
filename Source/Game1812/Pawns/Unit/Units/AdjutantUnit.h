#pragma once

#include "CoreMinimal.h"
#include "../BaseUnit.h"
#include "../Reports/UnitReport.h"
#include "../Components/Damageable.h"
#include "AdjutantUnit.generated.h"

UCLASS()
class GAME1812_API AAdjutantUnit : public ABaseUnit, public IDamageable
{
	GENERATED_BODY()
	
public:

	AAdjutantUnit();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAdjutantUnitOrder* CurrentOrder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUnitReport CollectedReports;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinDistanceToGiveOrder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DeathCooldown;

	FTimerHandle DeathCooldownTimer;

	UPROPERTY(VisibleAnywhere)
	TArray<struct FAssignedCombatUnitOrder> Orders;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMovementComplete();

	void MoveToNextTarget();

	UFUNCTION()
	void OnDeathCooldownEnd();

	void GiveReport();

	struct FAssignedCombatUnitOrder FindClosestTarget();

	UFUNCTION(BlueprintImplementableEvent)
	void OnReturnToHQ();

	UFUNCTION(BlueprintImplementableEvent)
	void OnOrderAssign();

public:

	bool IsOnDeathCooldown();
	bool IsInReachToInteractWithActor(AActor* Actor);
	
	void ForceReturnToHQ();

	//ABaseUnit class override
	class UUnitMovementComponent* GetMovementComponent();

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
	float GetDefense() override { return 0.f; };
	//
};
