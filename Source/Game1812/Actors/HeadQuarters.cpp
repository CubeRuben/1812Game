#include "HeadQuarters.h"

#include "../Pawns/Unit/BaseUnit.h"
#include "../Pawns/Unit/Units/AdjutantUnit.h"
#include "../CossacksGameInstance.h"

AHeadQuarters* AHeadQuarters::Instance = nullptr;

AHeadQuarters* AHeadQuarters::GetInstance()
{
	return Instance;
}

AHeadQuarters::AHeadQuarters()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	AdjutantsAmount = 3;
	RangeForCloseOrders = 50.f;
}

void AHeadQuarters::BeginPlay()
{
	Super::BeginPlay();
	
	Instance = this;

	UCossacksGameInstance* gameInstance = GetGameInstance<UCossacksGameInstance>();

	if (!gameInstance)
		return;

	for (int i = 0; i < AdjutantsAmount; i++) 
	{
		ABaseUnit* unit = SpawnUnit(gameInstance->GetAdjutantUnitClass());

		AAdjutantUnit* adjutantUnit = Cast<AAdjutantUnit>(unit);

		if (adjutantUnit) 
		{
			AvailableAdjutants.Add(adjutantUnit);
		}
	}
}

void AHeadQuarters::AddAdjutantUnit(AAdjutantUnit* AdjutantUnit)
{
	AvailableAdjutants.Add(AdjutantUnit);
}

void AHeadQuarters::RemoveAdjutantUnit(AAdjutantUnit* AdjutantUnit)
{
	AvailableAdjutants.Remove(AdjutantUnit);
}

void AHeadQuarters::SendOrders()
{
	for (int i = 0; i < UnitOrders.Num();) 
	{
		//Remove order, if unit is dead
		if (!UnitOrders[i].Unit.IsValid()) 
		{
			UnitOrders.RemoveAt(i);
			continue;
		}

		const FVector unitLocation = UnitOrders[i].Unit->GetActorLocation();
		const float distance = FVector::DistSquared2D(unitLocation, GetActorLocation());

		if (FMath::Pow(RangeForCloseOrders, 2) > distance)
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
	AvailableAdjutants.RemoveAt(0);

	UnitOrders.Empty();
}

void AHeadQuarters::AddOrderToAssign(class UCombatUnitOrder* UnitOrder, ABaseUnit* Unit)
{
	UnitOrders.RemoveAll([&](const FAssignedCombatUnitOrder& el) { return el.Unit == Unit; });

	UnitOrders.Add(FAssignedCombatUnitOrder(UnitOrder, Unit));
}

ABaseUnit* AHeadQuarters::SpawnUnit(TSubclassOf<class ABaseUnit> UnitClass)
{
	FVector point = GetActorLocation();

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return GetWorld()->SpawnActor<ABaseUnit>(UnitClass.Get(), point, FRotator(0, GetActorRotation().Yaw, 0), spawnParams);
}

bool AHeadQuarters::HaveAnyOrders()
{
	return UnitOrders.Num() != 0;
}

bool AHeadQuarters::HaveAnyAdjutants()
{
	return !AvailableAdjutants.IsEmpty();
}
