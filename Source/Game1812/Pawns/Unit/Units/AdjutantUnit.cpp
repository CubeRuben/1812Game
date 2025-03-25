#include "AdjutantUnit.h"

#include "CombatUnit.h"
#include "../Orders/AssignedUnitOrder.h"
#include "../Orders/UnitOrder.h"
#include "../Components/UnitMovementComponent.h"
#include "../Components/UnitReportComponent.h"
#include "../Components/UnitTerrainModifiersComponent.h"
#include "../../../OrdersSenderComponent.h"
#include "../../../ReportSystem/ReportSpawner.h"

AAdjutantUnit::AAdjutantUnit() :
	Contained(false)
{
	PrimaryActorTick.bCanEverTick = true;

	MovementSpeed = 100.f;
	RotationSpeed = 160.f;

	MinDistanceToGiveOrder = 15.f;

	DeathCooldown = 15.f;
}

void AAdjutantUnit::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->OnMovementEnd.AddDynamic(this, &AAdjutantUnit::OnMovementComplete);
}

void AAdjutantUnit::AssignOrder(UUnitOrder* NewOrder)
{
	if (IsOnDeathCooldown())
		return;

	CurrentOrder = Cast<UAdjutantUnitOrder>(NewOrder);

	if (!CurrentOrder)
		return;

	Orders = TArray<FAssignedCombatUnitOrder>(CurrentOrder->SentOrdersToUnits);
	MoveToNextTarget();
}

void AAdjutantUnit::OnBeingAddedToOrdersSender()
{
	Contained = true;
	MovementComponent->StopMoving();
	SetActorHiddenInGame(true);
}

void AAdjutantUnit::OnBeingRemovedFromOrdersSender()
{
	Contained = false;
	SetActorHiddenInGame(false);
}

void AAdjutantUnit::OnMovementComplete()
{
	if (IsOnDeathCooldown() || Contained)
		return;

	if (Orders.IsEmpty())
	{
		UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

		if (!ordersSender)
			return;

		if (IsInReachToInteractWithActor(ordersSender->GetOwner()))
		{
			ordersSender->ReturnAdjutantUnit(this);
			GiveReport();
			OnReturnToHQ();
			return;
		}

		MovementComponent->MoveToActor(ordersSender->GetOwner());
		return;
	}

	FAssignedCombatUnitOrder closestTarget = FindClosestTarget();

	if (IsInReachToInteractWithActor(closestTarget.Unit.Get()))
	{
		closestTarget.Unit->AssignOrder(closestTarget.UnitOrder);
		OnOrderAssign();
		Orders.Remove(closestTarget);

		ACombatUnit* combatUnit = Cast<ACombatUnit>(closestTarget.Unit);

		if (combatUnit && combatUnit->GetReportComponent())
			CollectedReports = CollectedReports + combatUnit->GetReportComponent()->RequestUnitReport();
	}

	MoveToNextTarget();
}

void AAdjutantUnit::MoveToNextTarget()
{
	if (Orders.IsEmpty()) 
	{
		UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

		if (!ordersSender)
			return;

		MovementComponent->ForceMoveTo(ordersSender->GetOwner()->GetActorLocation(), EUnitMovementType::Move);
		return;
	}

	MovementComponent->ForceMoveTo(FindClosestTarget().Unit->GetActorLocation(), EUnitMovementType::Move);
}

FAssignedCombatUnitOrder AAdjutantUnit::FindClosestTarget()
{
	FAssignedCombatUnitOrder closestUnit = Orders[0];

	for (int i = 0; i < Orders.Num(); i++)
	{
		if (!Orders[i].Unit.IsValid())
		{
			Orders.RemoveAt(i);
			i--;
			continue;
		}

		if (FVector::DistSquared2D(GetActorLocation(), Orders[i].Unit->GetActorLocation()) < FVector::DistSquared2D(GetActorLocation(), closestUnit.Unit->GetActorLocation()))
		{
			closestUnit = Orders[i];
		}
	}

	return closestUnit;
}

bool AAdjutantUnit::IsOnDeathCooldown()
{
	return GetWorldTimerManager().IsTimerActive(DeathCooldownTimer);
}

bool AAdjutantUnit::IsInReachToInteractWithActor(AActor* Actor)
{
	if (!Actor)
		return false;

	return FVector::DistSquared2D(GetActorLocation(), Actor->GetActorLocation()) < FMath::Pow(MinDistanceToGiveOrder, 2);
}

float AAdjutantUnit::ApplyDamage(IDamageable* Attacker, float Amount)
{
	if (Amount < 1.f)
		return 0.f;

	ForceReturnToHQ();
	return 1.f;
}

void AAdjutantUnit::ForceReturnToHQ()
{
	UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

	if (!ordersSender)
	{
		Destroy();
		return;
	}

	ordersSender->TakeAdjutantUnit(this);
	GetWorldTimerManager().SetTimer(DeathCooldownTimer, this, &AAdjutantUnit::OnDeathCooldownEnd, DeathCooldown);

	Orders.Empty();
	SetActorLocation(ordersSender->GetOwner()->GetActorLocation());
	MovementComponent->StopMoving();

	CollectedReports.Clear();
}

void AAdjutantUnit::OnDeathCooldownEnd()
{
	UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

	if (ordersSender)
		ordersSender->ReturnAdjutantUnit(this);
}

void AAdjutantUnit::GiveReport()
{
	AReportSpawner* reportSpawner = AReportSpawner::GetInstance();

	if (reportSpawner)
		reportSpawner->AddReport(CollectedReports);

	CollectedReports.Clear();
}

ECombatUnitType AAdjutantUnit::GetUnitType() const
{
	return ECombatUnitType::Cavalry;
}

FVector AAdjutantUnit::GetLocation()
{
	return GetActorLocation();
}

bool AAdjutantUnit::IsValidTarget()
{
	return !IsOnDeathCooldown();
}

UUnitOrder* AAdjutantUnit::GetCurrentOrder()
{
	return nullptr;
}

UUnitMovementComponent* AAdjutantUnit::GetMovementComponent()
{
	return MovementComponent;
}

float AAdjutantUnit::GetMovementSpeed() const
{
	return MovementSpeed * GetTerrainModifiers().MovementSpeedModifier;
}

float AAdjutantUnit::GetRotationSpeed() const
{
	return RotationSpeed * GetTerrainModifiers().RotationSpeedModifier;
}


