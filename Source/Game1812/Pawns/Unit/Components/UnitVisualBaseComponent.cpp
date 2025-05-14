#include "UnitVisualBaseComponent.h"

#include "../../../Actors/GlobalUnitCombatVisual.h"
#include "UnitVisualMeshComponent.h"

UUnitVisualBaseComponent::UUnitVisualBaseComponent()
{
}

void UUnitVisualBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	AGlobalUnitCombatVisual::GetInstance(GetWorld());
}

void UUnitVisualBaseComponent::BeginDestroy()
{
	Super::BeginDestroy();

	AGlobalUnitCombatVisual* const instance = AGlobalUnitCombatVisual::GetInstance();

	if (instance)
		instance->RemoveVisualComponent(this);
}

UUnitVisualMeshComponent* UUnitVisualBaseComponent::CreateVisualComponent(const FName& Name, UStaticMesh* Mesh)
{
	UUnitVisualMeshComponent* const component = NewObject<UUnitVisualMeshComponent>(GetOwner(), UUnitVisualMeshComponent::StaticClass(), Name);

	if (!component)
		return nullptr;

	component->RegisterComponent();
	component->SetStaticMesh(Mesh);
	component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetOwner()->AddInstanceComponent(component);
	
	return component;
}
