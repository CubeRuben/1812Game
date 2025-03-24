#include "EscortConvoyFollowPoint.h"

#include "../../Pawns/Unit/Units/CombatUnit.h"
#include "../../Pawns/Unit/Orders/UnitOrder.h"

AEscortConvoyFollowPoint::AEscortConvoyFollowPoint() :
	RandomEnemyOffset(50.0f)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
}

void AEscortConvoyFollowPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEscortConvoyFollowPoint::OnConvoyArrived()
{
	for (ACombatUnit* const& combatUnit : EnemyCombatUnits) 
	{
		if (!combatUnit)
			continue;

		UCombatUnitOrder* order = Cast<UCombatUnitOrder>(combatUnit->GetCurrentOrder());

		if (!order)
			continue;

		FVector orderLocation = GetActorLocation();
		orderLocation.X += FMath::FRandRange(-RandomEnemyOffset, RandomEnemyOffset);
		orderLocation.Y += FMath::FRandRange(-RandomEnemyOffset, RandomEnemyOffset);
		order->Location = orderLocation;

		combatUnit->AssignOrder(order);
	}
}
