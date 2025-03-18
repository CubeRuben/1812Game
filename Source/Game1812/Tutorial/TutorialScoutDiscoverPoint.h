#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../FogSystem/FogAffected.h"
#include "TutorialScoutDiscoverPoint.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPointDiscoverDelegate)

UCLASS()
class GAME1812_API ATutorialScoutDiscoverPoint : public AActor, public IFogAffected
{
	GENERATED_BODY()
	
public:	

	ATutorialScoutDiscoverPoint();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	virtual void BeginPlay() override;

	FOnPointDiscoverDelegate OnPointDiscover;

public:	

	bool GetPointActive() const;
	void SetPointActive(bool NewActive);

	FDelegateHandle BindOnPointDiscover(const FOnPointDiscoverDelegate::FDelegate& Delegate) { return OnPointDiscover.Add(Delegate); }
	void UnbindOnPointDiscover(const FDelegateHandle& DelegateHandle) { OnPointDiscover.Remove(DelegateHandle); }

	//IFogAffected Interface
	void OnBeingCoveredInFog() override {};
	void OnBeingRevealedFromFog() override;
	bool IsCoveredInFog() override { return true; }
	inline FVector GetWorldLocation() const override { return GetActorLocation(); };
	bool IsAffectedByFog() override { return GetPointActive(); }
	//
};
