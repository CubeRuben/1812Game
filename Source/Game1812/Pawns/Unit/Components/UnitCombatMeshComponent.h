#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "../../../FogSystem/FogAffected.h"
#include "UnitCombatMeshComponent.generated.h"

UCLASS()
class GAME1812_API UUnitCombatMeshComponent : public UStaticMeshComponent, public IFogAffected
{
	GENERATED_BODY()

public:

	UUnitCombatMeshComponent();

	//IFogAffected Interface
	void OnBeingCoveredInFog() override { SetVisibility(false); }
	void OnBeingRevealedFromFog() override { SetVisibility(true); }
	bool IsCoveredInFog() override { return !IsVisible(); }
	inline FVector GetWorldLocation() const override { return GetComponentLocation(); }
	//
	
};
