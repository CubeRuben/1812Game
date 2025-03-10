#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../FogSystem/FogAffected.h"
#include "DeadUnitsVisual.generated.h"

UCLASS()
class GAME1812_API ADeadUnitsVisual : public AActor, public IFogAffected
{
	GENERATED_BODY()
	
public:	

	ADeadUnitsVisual();

protected:

	static ADeadUnitsVisual* Instance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UDeadUnitMeshComponent*> DeadUnitMeshComponents;

	TArray<IFogAffected*> FogAffectedComponents;

	void BeginPlay() override;
	void BeginDestroy() override;

public:	

	static ADeadUnitsVisual* GetInstance() { return Instance; }
	static ADeadUnitsVisual* GetInstance(UWorld* World);

	void AddDeadMesh(const FVector& Location, const struct FCombatUnitVisual& CombatUnitVisual);

	//IFogAffected Interface
	void OnBeingCoveredInFog() override { }
	void OnBeingRevealedFromFog() override { }
	bool IsCoveredInFog() override { return false; }
	inline FVector GetWorldLocation() const override { return GetActorLocation(); }

	bool IsAffectedByFog() override { return false; }

	virtual TArray<IFogAffected*>* GetFogAffectedComponents() { return &FogAffectedComponents; }
	//

};
