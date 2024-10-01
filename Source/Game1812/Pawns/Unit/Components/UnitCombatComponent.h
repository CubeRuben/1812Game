#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../UnitTypeEnum.h"
#include "UObject/WeakInterfacePtr.h"
#include "UnitCombatComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageDealtDelegate, class ACombatUnit*, Unit, float, DealtDamage);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageTakenDelegate, class ACombatUnit*, Unit, float, TakenDamage);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTemporarilyDefeatDelegate);

UCLASS(Blueprintable, BlueprintType)
class GAME1812_API UUnitCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UUnitCombatComponent();

protected:

	class ACombatUnit* CombatUnitPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Combat")
	float HealthPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Combat")
	float Morale;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Combat")
	bool bIsTemporarilyDefeated;

	float TimeOfLastAttack;

	float TimeOfLastTakenDamage;

	TWeakInterfacePtr<class IDamageable> TargetedEnemy;

	virtual void BeginPlay() override;

	class UCombatUnitOrder* GetCombatUnitOrder();

	void SetTargetedEnemy(class IDamageable* NewTarget);

	void AddMorale(float Amount);

	UFUNCTION()
	void OnPawnMove(float Distance);
	void OnBeingAttackedBehaviour(class IDamageable* Attacker);

	void TryAttack(class IDamageable* Target);
	bool CanAttack(class IDamageable* Target);
	void Attack(class IDamageable* Target);

	void UpdateMoraleRestoration(float DeltaTime);
	void UpdateTempDefeat();
	void UpdateOrderBehaviour();
	void UpdateTargetAttack();

	void FindEnemiesInRange(TArray<class IDamageable*>& OutArray);
	class IDamageable* FindClosestEnemyInRange();

	FVector FindRetreatDirection();

	bool IsTargetInDetectionRange(IDamageable* Target);
	bool IsTargetInAttackRange(IDamageable* Target);
	
public:	

	UPROPERTY(BlueprintAssignable)
	FOnDamageDealtDelegate OnDamageDealt;

	UPROPERTY(BlueprintAssignable)
	FOnDamageTakenDelegate OnDamageTaken;

	UPROPERTY(BlueprintAssignable)
	FOnTemporarilyDefeatDelegate OnTemporarilyDefeat;

	void Init(struct FCombatUnitStats* UnitCombatStats);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float ApplyDamage(class IDamageable* Attacker, float DamageAmount);

	float GetAttackCooldown() const;
	float GetBaseDamage() const;
	float GetBaseDefense() const;
	float GetAttackRange() const;
	float GetDetectionRange() const;

	float GetMorale() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHPRatio() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMoraleRatio() const;

	float GetDamageMultiplier(ECombatUnitType AttackedUnitType) const;

	float CalculateDamage(IDamageable* AttackedTarget) const;
	float CalculateDefense() const;

	float CalculateMovementSpeed();
	float CalculateRotationSpeed();
	
};
