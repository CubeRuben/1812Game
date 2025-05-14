#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "../../../FogSystem/FogAffected.h"
#include "UnitVisualMeshComponent.generated.h"

UCLASS()
class GAME1812_API UUnitVisualMeshComponent : public UStaticMeshComponent, public IFogAffected
{
	GENERATED_BODY()

public:

	UUnitVisualMeshComponent();

	//IFogAffected Interface
	void OnBeingCoveredInFog() override { SetVisibility(false); }
	void OnBeingRevealedFromFog() override { SetVisibility(true); }
	bool IsCoveredInFog() override { return !IsVisible(); }
	inline FVector GetWorldLocation() const override { return GetComponentLocation(); }
	//
	
};
