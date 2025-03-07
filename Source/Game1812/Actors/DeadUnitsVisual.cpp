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

void ADeadUnitsVisual::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADeadUnitsVisual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<class IFogAffectedComponent*>* ADeadUnitsVisual::GetFogAffectedComponents()
{
	return nullptr;
}

