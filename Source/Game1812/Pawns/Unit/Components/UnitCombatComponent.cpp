#include "UnitCombatComponent.h"

#include "UnitMovementComponent.h"
#include "UnitTerrainModifiersComponent.h"
#include "Damageable.h"


#include "../../../DataAssets/CombatUnitDataAsset.h"
#include "../Units/CombatUnit.h"
#include "../Orders/UnitOrder.h"

#include <Kismet/GameplayStatics.h>
#include <Math/Quat.h>

UUnitCombatComponent::UUnitCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CombatUnitPawn = nullptr;
	TargetedEnemy = nullptr;

	HealthPoints = 0.f;
	Morale = 1.f;
	bIsTemporarilyDefeated = false;

	TimeOfLastAttack = -100.f;
	TimeOfLastTakenDamage = -100.f;
}

void UUnitCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatUnitPawn = Cast<ACombatUnit>(GetOwner());

	if (!CombatUnitPawn)
		DestroyComponent();

	CombatUnitPawn->GetMovementComponent()->OnMove.AddDynamic(this, &UUnitCombatComponent::OnPawnMove);
}

void UUnitCombatComponent::Init(FCombatUnitStats* UnitCombatStats)
{
	if (!UnitCombatStats)
		return;

	HealthPoints = UnitCombatStats->GetBaseHP();
}

void UUnitCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMoraleRestoration(DeltaTime);

	UpdateTempDefeat();

	if (bIsTemporarilyDefeated)
		return;

	UpdateOrderBehaviour();

	UpdateTargetAttack();
}

void UUnitCombatComponent::UpdateMoraleRestoration(float DeltaTime)
{
	if (CombatUnitPawn->GetMovementComponent()->IsMoving() || (TimeOfLastTakenDamage + 5.f > GetWorld()->GetTimeSeconds()))
		return;

	const float moraleRestorationSpeed = CombatUnitPawn->GetCombatUnitStats()->GetMoraleRestorationSpeed();
	const float moraleRestoreDelta = moraleRestorationSpeed * DeltaTime;

	AddMorale(moraleRestoreDelta);
}

void UUnitCombatComponent::UpdateTempDefeat()
{
	if (!bIsTemporarilyDefeated)
		return;

	if (Morale > 0.5f)
	{
		bIsTemporarilyDefeated = false;
		return;
	}

	UUnitMovementComponent* movementComponent = CombatUnitPawn->GetMovementComponent();

	if (movementComponent->IsMoving())
		return;

	const FVector retreatDirection = FindRetreatDirection();

	if (retreatDirection.IsNearlyZero())
		return;

	const FVector moveToLocation = CombatUnitPawn->GetActorLocation() + retreatDirection * 200.f + FVector(0, 0, 100.f);
	movementComponent->MoveTo(moveToLocation);
}

void UUnitCombatComponent::UpdateOrderBehaviour()
{
	UCombatUnitOrder* order = GetCombatUnitOrder();

	if (!order)
		return;

	if (order->UnitEnemyReaction == EUnitEnemyReaction::Attack)
	{
		//Try to find enemy
		//if (!TargetedEnemy.IsValid())
		{
			IDamageable* enemy = FindClosestEnemyInRange();

			if (!enemy)
			{
				CombatUnitPawn->GetMovementComponent()->MoveTo(order->Location, true);
				CombatUnitPawn->GetMovementComponent()->RotateTo(order->YawRotation);
				return;
			}

			SetTargetedEnemy(enemy);
		}
	}
}

void UUnitCombatComponent::UpdateTargetAttack()
{
	UUnitMovementComponent* movementComponent = CombatUnitPawn->GetMovementComponent();

	if (!TargetedEnemy.IsValid())
		return;

	if (!IsTargetInDetectionRange(TargetedEnemy.Get()) || !TargetedEnemy->IsValidTarget()) 
	{
		SetTargetedEnemy(nullptr);
		return;
	}

	//Walk to the enemy, if too far
	if (!IsTargetInAttackRange(TargetedEnemy.Get()))
	{
		movementComponent->MoveTo(TargetedEnemy->GetLocation());
		return;
	}

	//Stop when enemy is close
	if (movementComponent->IsMoving())
		movementComponent->StopMoving();

	const FQuat quat = FQuat::FindBetween(FVector::XAxisVector, TargetedEnemy->GetLocation() - CombatUnitPawn->GetLocation());
	movementComponent->RotateTo(FRotator(quat).Yaw);

	//TF2 soldier: ATTACK!
	TryAttack(TargetedEnemy.Get());
}

void UUnitCombatComponent::OnPawnMove(float Distance)
{
	float moraleLoss = CombatUnitPawn->GetCombatUnitStats()->GetDistanceForFullMoraleLoss();

	if (GetCombatUnitOrder()->bForcedMarch)
		moraleLoss *= 1.5f;

	if (moraleLoss != 0.f)
		AddMorale(-(Distance / moraleLoss));
}

void UUnitCombatComponent::OnBeingAttackedBehaviour(IDamageable* Attacker)
{
	if (!Attacker)
		return;

	if (!TargetedEnemy.IsValid())
	{
		SetTargetedEnemy(Attacker);
		return;
	}

	if (!IsTargetInAttackRange(TargetedEnemy.Get())) 
	{
		SetTargetedEnemy(Attacker);
	}
}

void UUnitCombatComponent::TryAttack(IDamageable* Target)
{
	if (CanAttack(Target))
	{
		Attack(Target);
	}
}

void UUnitCombatComponent::Attack(IDamageable* Target)
{
	TimeOfLastAttack = GetWorld()->GetTimeSeconds();

	const float damageDealt = Target->ApplyDamage(CombatUnitPawn, CalculateDamage(Target));

	OnDamageDealt.Broadcast(CombatUnitPawn, damageDealt);
}

bool UUnitCombatComponent::CanAttack(IDamageable* Target)
{
	const bool closeEnoughToEnemy = IsTargetInAttackRange(Target);
	const bool cooldownFinished = GetAttackCooldown() + TimeOfLastAttack < GetWorld()->GetTimeSeconds();

	return closeEnoughToEnemy && cooldownFinished;
}

bool UUnitCombatComponent::IsTargetInDetectionRange(IDamageable* Target)
{
	return FVector::DistSquared2D(Target->GetLocation(), CombatUnitPawn->GetActorLocation()) < FMath::Pow(GetDetectionRange(), 2);
}

bool UUnitCombatComponent::IsTargetInAttackRange(IDamageable* Target)
{
	return FVector::DistSquared2D(Target->GetLocation(), CombatUnitPawn->GetActorLocation()) < FMath::Pow(GetAttackRange(), 2);
}

float UUnitCombatComponent::ApplyDamage(IDamageable* Attacker, float DamageAmount)
{
	//Attack back
	TryAttack(Attacker);

	//Calculate total damage with defense
	const float totalDamage = FMath::Max(1, DamageAmount);
	HealthPoints -= totalDamage;

	//Destroy if no HP
	if (HealthPoints <= 0.f)
	{
		CombatUnitPawn->OnUnitDeath();
		CombatUnitPawn->Destroy();
		
		//Return applied damage
		const float totalPossibleDamage = totalDamage + HealthPoints;
		OnDamageTaken.Broadcast(CombatUnitPawn, totalPossibleDamage);
		return totalPossibleDamage;
	}

	//Reduce morale
	const float moraleLoss = CombatUnitPawn->GetCombatUnitStats()->GetMoraleLossDueToLosses();
	AddMorale(-(totalDamage * moraleLoss));

	//Make defeated if low morale
	if (Morale < 0.05f) 
	{
		bIsTemporarilyDefeated = true;
		OnTemporarilyDefeat.Broadcast();
	}

	//Reset time of last taken damage
	TimeOfLastTakenDamage = GetWorld()->GetTimeSeconds();

	//React on being attacker
	OnBeingAttackedBehaviour(Attacker);

	//Return applied damage
	OnDamageTaken.Broadcast(CombatUnitPawn, totalDamage);
	return totalDamage;
}

float UUnitCombatComponent::CalculateDamage(IDamageable* AttackedTarget) const
{
	return FMath::Sqrt(Morale) * FMath::Max(0.f, GetBaseDamage() - AttackedTarget->GetDefense()) * HealthPoints * GetDamageMultiplier(AttackedTarget->GetUnitType()) * CombatUnitPawn->GetTerrainModifiers().DamageModifier;
}

float UUnitCombatComponent::CalculateDefense() const
{
	return FMath::Sqrt(Morale) * GetBaseDefense();
}

float UUnitCombatComponent::GetDamageMultiplier(ECombatUnitType AttackedUnitType) const
{
	const float* customMultiplier = CombatUnitPawn->GetCombatUnitStats()->GetDamageMultipliers().Find(AttackedUnitType);

	if (customMultiplier)
		return *customMultiplier;

	return 1.f;
}

float UUnitCombatComponent::CalculateMovementSpeed()
{
	const float maxSpeed = CombatUnitPawn->GetCombatUnitStats()->GetMaxMovementSpeed();

	if (bIsTemporarilyDefeated)
		return maxSpeed * 1.25f;
	
	const float minSpeed = CombatUnitPawn->GetCombatUnitStats()->GetMinMovementSpeed();
	const float speed = Morale * (maxSpeed - minSpeed) + minSpeed;
	const float clampedSpeed = FMath::Clamp(speed, minSpeed, maxSpeed);

	if (GetCombatUnitOrder()->bForcedMarch)
		return clampedSpeed * 1.25f;

	return clampedSpeed;
}

float UUnitCombatComponent::CalculateRotationSpeed()
{
	const float baseRotationSpeed = CombatUnitPawn->GetCombatUnitStats()->GetRotationSpeed();

	if (bIsTemporarilyDefeated)
		return baseRotationSpeed * 3.5f;

	if (GetCombatUnitOrder()->bForcedMarch)
		return baseRotationSpeed * 1.25f;

	return baseRotationSpeed;
}

void UUnitCombatComponent::FindEnemiesInRange(TArray<IDamageable*>& OutArray)
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UDamageable::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		if (FVector::DistSquared2D(actor->GetActorLocation(), CombatUnitPawn->GetActorLocation()) > FMath::Pow(GetDetectionRange(), 2))
			continue;

		IDamageable* damageable = Cast<IDamageable>(actor);

		if (!damageable)
			continue;

		if (!damageable->IsValidTarget())
			continue;

		if (!damageable->IsEnemy(CombatUnitPawn->GetTeam()))
			continue;

		OutArray.Add(damageable);
	}
}

IDamageable* UUnitCombatComponent::FindClosestEnemyInRange()
{
	TArray<IDamageable*> damageables;

	FindEnemiesInRange(damageables);

	if (damageables.IsEmpty())
		return nullptr;

	IDamageable* closestDamageable = damageables[0];

	for (IDamageable* damageable : damageables)
	{
		if (FVector::DistSquared2D(damageable->GetLocation(), CombatUnitPawn->GetActorLocation()) < FVector::DistSquared2D(closestDamageable->GetLocation(), CombatUnitPawn->GetActorLocation()))
		{
			closestDamageable = damageable;
		}
	}

	return closestDamageable;
}

FVector UUnitCombatComponent::FindRetreatDirection()
{
	TArray<IDamageable*> enemies;

	FindEnemiesInRange(enemies);

	if (enemies.IsEmpty())
		return FVector::ZeroVector;

	FVector sumDirection;

	for (IDamageable* enemy : enemies) 
	{
		FVector direction = CombatUnitPawn->GetActorLocation() - enemy->GetLocation();

		direction.X = 1 / direction.X;
		direction.Y = 1 / direction.Y;

		sumDirection += direction;
	}

	return sumDirection.GetSafeNormal2D();
}

UCombatUnitOrder* UUnitCombatComponent::GetCombatUnitOrder()
{
	return Cast<UCombatUnitOrder>(CombatUnitPawn->GetCurrentOrder());
}

float UUnitCombatComponent::GetAttackCooldown() const
{
	return CombatUnitPawn->GetCombatUnitStats()->GetAttackCooldown();
}

float UUnitCombatComponent::GetBaseDamage() const
{
	return CombatUnitPawn->GetCombatUnitStats()->GetBaseDamage();
}

float UUnitCombatComponent::GetBaseDefense() const
{
	return CombatUnitPawn->GetCombatUnitStats()->GetBaseDefense();
}

float UUnitCombatComponent::GetAttackRange() const
{
	return CombatUnitPawn->GetCombatUnitStats()->GetAttackDistance();
}

float UUnitCombatComponent::GetDetectionRange() const
{
	return CombatUnitPawn->GetCombatUnitStats()->GetEnemyDetectionRange();
}

float UUnitCombatComponent::GetMorale() const
{
	return Morale;
}

float UUnitCombatComponent::GetHPRatio() const
{
	return HealthPoints / CombatUnitPawn->GetCombatUnitStats()->GetBaseHP();
}

float UUnitCombatComponent::GetMoraleRatio() const
{
	return Morale;
}

void UUnitCombatComponent::AddMorale(float Amount)
{
	Morale = FMath::Clamp(Morale + Amount, 0.f, 1.f);
}

void UUnitCombatComponent::SetTargetedEnemy(IDamageable* NewTarget)
{
	TargetedEnemy = NewTarget;
}