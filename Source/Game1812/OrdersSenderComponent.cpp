#include "OrdersSenderComponent.h"

#include "Pawns/Unit/BaseUnit.h"
#include "Pawns/Unit/Units/CombatUnit.h"
#include "Pawns/Unit/Units/AdjutantUnit.h"
#include "Pawns/Unit/Orders/UnitOrder.h"
#include "CossacksGameInstance.h"
#include "FogSystem/FogOfWar.h"

UOrdersSenderComponent* UOrdersSenderComponent::Instance = nullptr;

UOrdersSenderComponent::UOrdersSenderComponent() :
	AdjutantsAmount(3), CloseOrdersRange(150.0f), AllowedToSendOrders(true), DiscoverRange(8.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOrdersSenderComponent::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;

	SpawnAdjutants();
}

void UOrdersSenderComponent::BeginDestroy()
{
	Super::BeginDestroy();

	Instance = nullptr;
}

void UOrdersSenderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FVector actorLocation = GetOwner()->GetActorLocation();

	for (ABaseUnit* const& unit : ContainedUnit) 
	{
		if (!unit)
			continue;

		unit->SetActorLocation(actorLocation);
	}
}

void UOrdersSenderComponent::UpdateFogOfWar()
{
	AFogOfWar* const fogOfWar = AFogOfWar::GetInstance();

	if (!fogOfWar)
		return;

	fogOfWar->SetConstantDiscoveredArea(GetOwner()->GetActorLocation(), DiscoverRange);
}

void UOrdersSenderComponent::SpawnAdjutants()
{
	UCossacksGameInstance* const gameInstance = GetWorld()->GetGameInstance<UCossacksGameInstance>();

	if (!gameInstance)
		return;

	for (int i = 0; i < AdjutantsAmount; i++)
	{
		ABaseUnit* const unit = SpawnUnit(gameInstance->GetAdjutantUnitClass());
		AAdjutantUnit* const adjutantUnit = Cast<AAdjutantUnit>(unit);

		if (!adjutantUnit)
			continue;
		
		ReturnAdjutantUnit(adjutantUnit);
	}
}

ABaseUnit* UOrdersSenderComponent::SpawnUnit(TSubclassOf<ABaseUnit> UnitClass)
{
	FVector point = GetOwner()->GetActorLocation();

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABaseUnit* const unit = GetWorld()->SpawnActor<ABaseUnit>(UnitClass.Get(), point, FRotator::ZeroRotator, spawnParams);

	if (unit->IsA(ACombatUnit::StaticClass()))
	{
		const float offset = 75.0f;

		point.X += FMath::FRandRange(-offset, offset);
		point.Y += FMath::FRandRange(-offset, offset);

		point.Z += 25.0f;

		unit->SetActorLocation(point);

		UCombatUnitOrder* const order = Cast<UCombatUnitOrder>(unit->GetCurrentOrder());

		if (order)
		{
			order->Location = point;
			unit->AssignOrder(order);
		}
	}

	return unit;
}

void UOrdersSenderComponent::SendOrders()
{
	if (!CanSendOrders())
		return;

	OnOrdersSendEvent.Broadcast();

	const FVector actorLocation = GetOwner()->GetActorLocation();

	for (int i = 0; i < UnitOrders.Num();)
	{
		//Remove order, if unit is dead
		if (!UnitOrders[i].Unit.IsValid())
		{
			UnitOrders.RemoveAt(i);
			continue;
		}

		const FVector unitLocation = UnitOrders[i].Unit->GetActorLocation();
		const float distance = FVector::DistSquared2D(unitLocation, actorLocation);

		if (FMath::Pow(CloseOrdersRange, 2) > distance)
		{
			UnitOrders[i].Unit->AssignOrder(UnitOrders[i].UnitOrder);
			UnitOrders.RemoveAt(i);
			continue;
		}

		i++;
	}

	if (!HaveAnyAdjutants() || !HaveAnyOrders())
		return;

	UAdjutantUnitOrder* unitOrder = NewObject<UAdjutantUnitOrder>();
	unitOrder->SentOrdersToUnits = UnitOrders;

	AvailableAdjutants[0]->AssignOrder(unitOrder);
	TakeAdjutantUnit(AvailableAdjutants[0]);

	UnitOrders.Empty();
}

void UOrdersSenderComponent::ReturnAdjutantUnit(AAdjutantUnit* AdjutantUnit)
{
	if (!AdjutantUnit)
		return;
		
	AvailableAdjutants.Add(AdjutantUnit);
	AddContainedUnit(AdjutantUnit);
}

void UOrdersSenderComponent::TakeAdjutantUnit(AAdjutantUnit* AdjutantUnit)
{
	if (!AdjutantUnit)
		return;

	AvailableAdjutants.Remove(AdjutantUnit);
	RemoveContainedUnit(AdjutantUnit);
}

void UOrdersSenderComponent::AddContainedUnit(ABaseUnit* Unit)
{
	if (!Unit)
		return;

	ContainedUnit.Add(Unit);
	Unit->OnBeingAddedToOrdersSender();
}

void UOrdersSenderComponent::RemoveContainedUnit(ABaseUnit* Unit)
{
	if (!Unit)
		return;

	ContainedUnit.Remove(Unit);
	Unit->OnBeingRemovedFromOrdersSender();
}

void UOrdersSenderComponent::AddAssignedOrder(class UCombatUnitOrder* UnitOrder, ABaseUnit* Unit)
{
	UnitOrders.RemoveAll([&](const FAssignedCombatUnitOrder& el) { return el.Unit == Unit; });

	UnitOrders.Add(FAssignedCombatUnitOrder(UnitOrder, Unit));
}

bool UOrdersSenderComponent::HaveAnyOrders()
{
	return !UnitOrders.IsEmpty();
}

bool UOrdersSenderComponent::HaveAnyAdjutants()
{
	return !AvailableAdjutants.IsEmpty();
}

bool UOrdersSenderComponent::CanSendOrders()
{
	return HaveAnyOrders() && AllowedToSendOrders;
}

