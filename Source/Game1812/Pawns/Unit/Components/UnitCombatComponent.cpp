#include "UnitCombatComponent.h"

#include "UnitMovementComponent.h"
#include "UnitTerrainModifiersComponent.h"
#include "Damageable.h"

#include "../../../Actors/Pieces/Piece.h"
#include "../../../DataAssets/CombatUnitDataAsset.h"
#include "../Units/CombatUnit.h"
#include "../Orders/UnitOrder.h"

#include <Kismet/GameplayStatics.h>
#include <Math/Quat.h>

UUnitCombatComponent::UUnitCombatComponent() :
	CombatUnitPawn(nullptr),
	HealthPoints(0.0f),
	Morale(1.0f),
	bIsTemporarilyDefeated(false),
	TimeOfLastAttack(-100.0f),
	TimeOfLastTakenDamage(-100.0f),
	TargetedEnemy(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUnitCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatUnitPawn = Cast<ACombatUnit>(GetOwner());

	if (!CombatUnitPawn)
		DestroyComponent();

	CombatUnitPawn->GetMovementComponent()->OnMove.AddDynamic(this, &UUnitCombatComponent::OnPawnMove);
}

void UUnitCombatComponent::Init(const struct FCombatUnitStats& UnitCombatStats)
{
	HealthPoints = UnitCombatStats.GetBaseHP();
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

	const float moraleRestorationSpeed = CombatUnitPawn->GetCombatUnitStats().GetMoraleRestorationSpeed();
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
	movementComponent->MoveTo(moveToLocation, EUnitMovementType::Move);
}

void UUnitCombatComponent::UpdateOrderBehaviour()
{
	UCombatUnitOrder* order = GetCombatUnitOrder();

	if (!order)
		return;

	if (order->ReorganizationType != EUnitReorganization::None)
	{
		ReorganizeUpdate();
		return;
	}

	if (order->UnitEnemyReaction == EUnitEnemyReaction::Attack)
	{
		//Try to find enemy
		//if (!TargetedEnemy.IsValid())
		{
			IDamageable* enemy = FindClosestEnemyInRange();

			if (!enemy)
			{
				CombatUnitPawn->GetMovementComponent()->MoveTo(order->Location, EUnitMovementType::Move);
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
		movementComponent->MoveTo(TargetedEnemy->GetLocation(), EUnitMovementType::Attack);
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

void UUnitCombatComponent::ReorganizeUpdate()
{
	UCombatUnitOrder* order = GetCombatUnitOrder();

	if (!order->UnitToReorganizeWith)
	{
		order->ReorganizationType = EUnitReorganization::None;
		return;
	}

	if (!IsInReachToReorganize(order->UnitToReorganizeWith))
	{
		const FVector otherUnitLocation = order->UnitToReorganizeWith->GetActorLocation();
		CombatUnitPawn->GetMovementComponent()->MoveTo(otherUnitLocation, EUnitMovementType::Move);
		return;
	}

	switch (order->ReorganizationType)
	{
	case EUnitReorganization::Combine:
		OnCombineReorganization();
		break;
	case EUnitReorganization::RedistributeEvenly:
		OnRedistributeEvenlyReorganization();
		break;
	default:
		break;
	}
}

void UUnitCombatComponent::OnCombineReorganization()
{
	UCombatUnitOrder* order = GetCombatUnitOrder();
	UUnitCombatComponent* otherCombatComponent = order->UnitToReorganizeWith->GetCombatComponent();

	//Saving stats before changes
	const float morale = Morale;
	const float hp = HealthPoints;
	const float otherMorale = otherCombatComponent->GetMorale();
	const float otherHp = otherCombatComponent->GetHealthPoints();

	//Applying stats changes
	otherCombatComponent->ApplyHeal(hp);
	otherCombatComponent->SetMorale((morale * hp + otherMorale * otherHp) / (hp + otherHp) * 0.8f);

	//Killing unit
	CombatUnitPawn->Kill();
}

void UUnitCombatComponent::OnRedistributeEvenlyReorganization()
{
	UCombatUnitOrder* order = GetCombatUnitOrder();
	UUnitCombatComponent* otherCombatComponent = order->UnitToReorganizeWith->GetCombatComponent();

	//Saving stats before changes
	const float morale = Morale;
	const float hp = HealthPoints;
	const float otherMorale = otherCombatComponent->GetMorale();
	const float otherHp = otherCombatComponent->GetHealthPoints();
	const float combinedHp = hp + otherHp;
	const float averageHp = combinedHp / 2.0f;

	//Applying stats changes
	SetHealthPoints(averageHp);
	otherCombatComponent->SetHealthPoints(averageHp);

	if (otherHp - averageHp > 0.0f)
	{
		otherCombatComponent->SetMorale((morale * hp + otherMorale * (averageHp - otherHp)) / averageHp * 0.8f);
	}
	else
	{
		SetMorale((morale * (averageHp - hp) + otherMorale * otherHp) / averageHp * 0.8f);
	}

	order->ClearReorganizationOrder();
}

void UUnitCombatComponent::OnPawnMove(float Distance)
{
	float moraleLoss = CombatUnitPawn->GetCombatUnitStats().GetDistanceForFullMoraleLoss();

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

bool UUnitCombatComponent::IsInReachToReorganize(ACombatUnit* OtherUnit)
{
	return FVector::DistSquared2D(OtherUnit->GetActorLocation(), CombatUnitPawn->GetActorLocation()) < FMath::Pow(10.0f, 2);
}

float UUnitCombatComponent::ApplyDamage(IDamageable* Attacker, float DamageAmount)
{
	//Attack back
	TryAttack(Attacker);

	//Calculate total damage with defense
	const float totalDamage = FMath::Max(1, DamageAmount);
	ApplyDamage(totalDamage);

	//Destroy if no HP
	if (HealthPoints <= 0.0f)
	{
		CombatUnitPawn->OnUnitDeath();
		CombatUnitPawn->Destroy();
		
		//Return applied damage
		const float totalPossibleDamage = totalDamage + HealthPoints;
		OnDamageTaken.Broadcast(CombatUnitPawn, totalPossibleDamage);
		return totalPossibleDamage;
	}

	//Reduce morale
	const float moraleLoss = CombatUnitPawn->GetCombatUnitStats().GetMoraleLossDueToLosses();
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
	const float* customMultiplier = CombatUnitPawn->GetCombatUnitStats().GetDamageMultipliers().Find(AttackedUnitType);

	if (customMultiplier)
		return *customMultiplier;

	return 1.f;
}

float UUnitCombatComponent::CalculateMovementSpeed()
{
	const float maxSpeed = CombatUnitPawn->GetCombatUnitStats().GetMaxMovementSpeed();

	if (bIsTemporarilyDefeated)
		return maxSpeed * 1.25f;
	
	const float minSpeed = CombatUnitPawn->GetCombatUnitStats().GetMinMovementSpeed();
	const float speed = Morale * (maxSpeed - minSpeed) + minSpeed;
	const float clampedSpeed = FMath::Clamp(speed, minSpeed, maxSpeed);

	if (GetCombatUnitOrder()->bForcedMarch)
		return clampedSpeed * 1.25f;

	return clampedSpeed;
}

float UUnitCombatComponent::CalculateRotationSpeed()
{
	const float baseRotationSpeed = CombatUnitPawn->GetCombatUnitStats().GetRotationSpeed();

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
	return CombatUnitPawn->GetCombatUnitStats().GetAttackCooldown();
}

float UUnitCombatComponent::GetBaseDamage() const
{
	return CombatUnitPawn->GetCombatUnitStats().GetBaseDamage();
}

float UUnitCombatComponent::GetBaseDefense() const
{
	return CombatUnitPawn->GetCombatUnitStats().GetBaseDefense();
}

float UUnitCombatComponent::GetAttackRange() const
{
	return CombatUnitPawn->GetCombatUnitStats().GetAttackDistance();
}

float UUnitCombatComponent::GetDetectionRange() const
{
	return CombatUnitPawn->GetCombatUnitStats().GetEnemyDetectionRange();
}

void UUnitCombatComponent::SetHealthPointsClamped(float NewHealthPoints)
{
	HealthPoints = FMath::Clamp(NewHealthPoints, 0.0f, CombatUnitPawn->GetCombatUnitStats().GetBaseHP());
}

void UUnitCombatComponent::SetHealthPoints(float NewHealthPoints)
{
	SetHealthPointsClamped(NewHealthPoints);
	OnHealthPointsChange.Broadcast(HealthPoints, false);
}

void UUnitCombatComponent::ApplyHeal(float Amount)
{
	SetHealthPointsClamped(HealthPoints + Amount);
	OnHealthPointsChange.Broadcast(HealthPoints, false);
}

void UUnitCombatComponent::ApplyDamage(float Amount)
{
	SetHealthPointsClamped(HealthPoints - Amount);
	OnHealthPointsChange.Broadcast(HealthPoints, true);
}

float UUnitCombatComponent::GetHealthPointsRatio() const
{
	return HealthPoints / CombatUnitPawn->GetCombatUnitStats().GetBaseHP();
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

#if WITH_EDITOR
void UUnitCombatComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (propertyName == GET_MEMBER_NAME_CHECKED(UUnitCombatComponent, HealthPoints))
	{
		SetHealthPointsClamped(HealthPoints);
		OnHealthPointsChange.Broadcast(HealthPoints, true);
	}
}
#endif