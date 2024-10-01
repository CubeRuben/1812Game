#include "ScoutUnit.h"

#include "../Orders/UnitOrder.h"
#include "../../../FogSystem/FogOfWar.h"
#include "../../../Actors/HeadQuarters.h"
#include "../Components/UnitMovementComponent.h"
#include "../Components/UnitTerrainModifiersComponent.h"


AScoutUnit::AScoutUnit() 
{
	PrimaryActorTick.bCanEverTick = true;

	MovementSpeed = 100;
	RotationSpeed = 160;
}

void AScoutUnit::BeginPlay() 
{
	Super::BeginPlay();

	DiscoveredArea.SetDimensions(AFogOfWar::GetInstance()->GetDimensions());
	DiscoveredArea.Clear(FVector4f::Zero());

	MovementComponent->OnMovementEnd.AddDynamic(this, &AScoutUnit::OnMovementComplete);
}

void AScoutUnit::OnMovementComplete() 
{
	if (ExplorationLocations.IsEmpty())
	{
		if (OnMovementEnd.IsBound())
			OnMovementEnd.Broadcast();

		if (!AFogOfWar::GetInstance())
			return;

		AFogOfWar::GetInstance()->AddDiscoveredArea(DiscoveredArea);
		DiscoveredArea.Clear(FVector4f::Zero());

		OnReturnToHQ();
		return;
	}

	FVector location;
	ExplorationLocations.Dequeue(location);
	MovementComponent->MoveTo(location, true);
}

void AScoutUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementComponent->IsMoving())
	{
		if (!AFogOfWar::GetInstance())
			return;

		ApplyCircularBrushToImage(DiscoveredArea, AFogOfWar::GetInstance()->LocationToIndex(GetActorLocation()), AFogOfWar::GetInstance()->GetScoutRange(), FVector4f::One());
	}
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

	if (MovementComponent->IsMoving())
		return;

	for (const FVector& location : CurrentOrder->ExplorationLocations)
	{
		ExplorationLocations.Enqueue(location);
	}

	ExplorationLocations.Enqueue(AHeadQuarters::GetInstance()->GetActorLocation());

	if (OnMovementStart.IsBound())
		OnMovementStart.Broadcast();

	FVector firstLocation;
	ExplorationLocations.Dequeue(firstLocation);
	MovementComponent->MoveTo(firstLocation, true);
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

UUnitMovementComponent* AScoutUnit::GetMovementComponent()
{
	return MovementComponent;
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