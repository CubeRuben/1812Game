#include "DeadUnitMeshComponent.h"

UDeadUnitMeshComponent::UDeadUnitMeshComponent()
{
}

void UDeadUnitMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	SetVisibility(false);
}
