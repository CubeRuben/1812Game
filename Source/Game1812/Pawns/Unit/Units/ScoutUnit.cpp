#include "ScoutUnit.h"

#include "../Orders/UnitOrder.h"
#include "../../../OrdersSenderComponent.h"
#include "../../../FogSystem/FogOfWar.h"
#include "../../../Actors/HeadQuarters.h"
#include "../Components/UnitMovementComponent.h"
#include "../Components/UnitTerrainModifiersComponent.h"

AScoutUnit::AScoutUnit() :
	Contained(false)
{
	PrimaryActorTick.bCanEverTick = true;

	MovementSpeed = 100;
	RotationSpeed = 160;
}

void AScoutUnit::BeginPlay() 
{
	Super::BeginPlay();

	UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

	if (ordersSender)
		ordersSender->AddContainedUnit(this);

	DiscoveredArea.SetDimensions(AFogOfWar::GetInstance()->GetDimensions());
	DiscoveredArea.Clear(FVector4f::Zero());

	MovementComponent->OnMovementEnd.AddDynamic(this, &AScoutUnit::OnMovementComplete);
}

void AScoutUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementComponent->IsMoving())
	{
		AFogOfWar* const fogOfWar = AFogOfWar::GetInstance();

		if (!fogOfWar)
			return;

		ApplyCircularBrushToImage(DiscoveredArea, fogOfWar->LocationToIndex(GetActorLocation()), fogOfWar->GetScoutRange(), FVector4f::One());
	}
}

void AScoutUnit::OnBeingAddedToOrdersSender()
{
	Contained = true;
	MovementComponent->StopMoving();
	SetActorHiddenInGame(true);
}

void AScoutUnit::OnBeingRemovedFromOrdersSender()
{
	Contained = false;
	SetActorHiddenInGame(false);
}

void AScoutUnit::OnMovementComplete()
{
	if (Contained)
		return;

	if (!ExplorationLocations.IsEmpty())
	{
		FVector location;
		ExplorationLocations.Dequeue(location);
		MovementComponent->ForceMoveTo(location, EUnitMovementType::Move);
		return;
	}

	UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

	if (!ordersSender)
		return;

	const float squaredDistanceToOrdersSender = FVector::DistSquared2D(GetActorLocation(), ordersSender->GetOwner()->GetActorLocation());
	const float squaredDistanceToReturn = FMath::Square(15.0f);

	if (squaredDistanceToOrdersSender > squaredDistanceToReturn)
	{
		MovementComponent->ForceMoveTo(ordersSender->GetOwner()->GetActorLocation(), EUnitMovementType::Move);
		return;
	}

	ordersSender->AddContainedUnit(this);

	OnMovementEnd.Broadcast();
	OnReturnToHQ();

	AFogOfWar* const fogOfWar = AFogOfWar::GetInstance();

	if (!fogOfWar)
		return;

	fogOfWar->AddDiscoveredArea(DiscoveredArea);

	DiscoveredArea.Clear(FVector4f::Zero());
}

float AScoutUnit::PredictMovementTime()
{
	if (ExplorationLocations.IsEmpty())
		return 0.0f;

	float totalDistance = 0.0f;

	TArray<FVector> savedLocations;

	do
	{
		FVector location;

		ExplorationLocations.Dequeue(location);

		if (savedLocations.IsEmpty()) 
		{
			totalDistance += FVector::Dist2D(GetActorLocation(), location);
		}
		else 
		{
			totalDistance += FVector::Dist2D(savedLocations.Last(), location);
		}

		savedLocations.Add(location);
	} 
	while (!ExplorationLocations.IsEmpty());

	for (FVector el : savedLocations) 
	{
		ExplorationLocations.Enqueue(el);
	}

	return totalDistance / GetMovementSpeed();
}

void AScoutUnit::AssignOrder(UUnitOrder* NewOrder)
{
	CurrentOrder = Cast<UScoutUnitOrder>(NewOrder);

	if (!CurrentOrder)
		return;

	if (MovementComponent->IsMoving())
		return;

	UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

	if (ordersSender)
		ordersSender->RemoveContainedUnit(this);

	for (const FVector& location : CurrentOrder->ExplorationLocations)
	{
		ExplorationLocations.Enqueue(location);
	}

	if (OnMovementStart.IsBound())
		OnMovementStart.Broadcast();

	FVector firstLocation;
	ExplorationLocations.Dequeue(firstLocation);
	MovementComponent->ForceMoveTo(firstLocation, EUnitMovementType::Move);
}

float AScoutUnit::ApplyDamage(IDamageable* Attacker, float Amount)
{
	if (Amount < 1.f)
		return 0.f;
	
	OnUnitDeath();
	Destroy();
	return 1.f;
}

ECombatUnitType AScoutUnit::GetUnitType() const
{
	return ECombatUnitType::Cavalry;
}

FVector AScoutUnit::GetLocation()
{
	return GetActorLocation();
}

bool AScoutUnit::IsValidTarget()
{
	return true;
}

float AScoutUnit::GetDefense()
{
	return 0.0f;
}

UUnitOrder* AScoutUnit::GetCurrentOrder()
{
	return CurrentOrder;
}

float AScoutUnit::GetMovementSpeed() const
{
	return MovementSpeed * GetTerrainModifiers().MovementSpeedModifier;
}

float AScoutUnit::GetRotationSpeed() const
{
	return RotationSpeed * GetTerrainModifiers().RotationSpeedModifier;
}

void AScoutUnit::OnReturnToHQ_Implementation() {}