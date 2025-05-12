#include "UnitVisualBaseComponent.h"

#include "../../../Actors/GlobalUnitCombatVisual.h"

UUnitVisualBaseComponent::UUnitVisualBaseComponent()
{
}

void UUnitVisualBaseComponent::BeginDestroy()
{
	Super::BeginDestroy();

	AGlobalUnitCombatVisual* const instance = AGlobalUnitCombatVisual::GetInstance();

	if (instance)
		instance->RemoveVisualComponent(this);
}
