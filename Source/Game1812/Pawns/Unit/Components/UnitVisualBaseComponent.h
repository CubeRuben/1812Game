#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitVisualBaseComponent.generated.h"

UCLASS(Abstract)
class GAME1812_API UUnitVisualBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UUnitVisualBaseComponent();

protected:

	virtual void BeginDestroy() override;

public:

	virtual void UpdateVisual(float DeltaTime) {};

};
