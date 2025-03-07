#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../FogSystem/FogAffectedActor.h"
#include "DeadUnitsVisual.generated.h"

UCLASS()
class GAME1812_API ADeadUnitsVisual : public AActor, public IFogAffectedActor
{
	GENERATED_BODY()
	
public:	

	ADeadUnitsVisual();

protected:

	static ADeadUnitsVisual* Instance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class IFogAffectedComponent*> FogAffected;

	virtual void BeginPlay() override;

public:	

	static ADeadUnitsVisual* GetInstance() { return Instance; }
	static void CreateInstance(UWorld* World);

	virtual void Tick(float DeltaTime) override;

	// IFogAffectedActor
	virtual void OnBeingCoveredInFog() { }
	virtual void OnBeingRevealedFromFog() { }
	virtual bool IsCoveredInFog() { return false; };

	virtual TArray<class IFogAffectedComponent*>* GetFogAffectedComponents();
	//

};
