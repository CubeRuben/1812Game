#include "DeadUnitsVisual.h"

ADeadUnitsVisual* ADeadUnitsVisual::Instance = nullptr;

void ADeadUnitsVisual::CreateInstance(UWorld* World)
{
	Instance = World->SpawnActor<ADeadUnitsVisual>(ADeadUnitsVisual::StaticClass());
}

ADeadUnitsVisual::ADeadUnitsVisual()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ADeadUnitsVisual::AddDeadMesh(const FVector& Location, const FCombatUnitVisual& CombatUnitVisual)
{
	DrawDebugPoint(GetWorld(), Location, 5.0f, FColor::Red, true, 30.0f);
}
