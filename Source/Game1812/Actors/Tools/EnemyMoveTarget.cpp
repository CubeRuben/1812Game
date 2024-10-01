#include "EnemyMoveTarget.h"

#include "Kismet/GameplayStatics.h"
#include "../../CossacksGameMode.h"
#include "../../Pawns/Unit/Units/CombatUnit.h"
#include "../../Pawns/Unit/Orders/UnitOrder.h"

AEnemyMoveTarget::AEnemyMoveTarget()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	TimeToMove = 20.f;
	MoveAreaBound = 50.f;
}

void AEnemyMoveTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACossacksGameMode* gameMode = Cast<ACossacksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!gameMode)
		return;
	
	if (TimeToMove > gameMode->GetGameTimeMinutes()) 
		return;

	for (TWeakObjectPtr<ACombatUnit> unit : UnitsToMove) 
	{
		if (!unit.IsValid())
			continue;

		UCombatUnitOrder* order = NewObject<UCombatUnitOrder>(UCombatUnitOrder::StaticClass());

		FVector location = GetActorLocation();

		location.X += FMath::RandRange(-MoveAreaBound, MoveAreaBound);
		location.Y += FMath::RandRange(-MoveAreaBound, MoveAreaBound);

		order->Location = location;

		unit->AssignOrder(order);
	}

	Destroy();
}

