#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscortConvoyFollowPoint.generated.h"

UCLASS()
class GAME1812_API AEscortConvoyFollowPoint : public AActor
{
	GENERATED_BODY()
	
public:	

	AEscortConvoyFollowPoint();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class ACombatUnit*> EnemyCombatUnits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RandomEnemyOffset;

	virtual void BeginPlay() override;

public:	

	void OnConvoyArrived();
};
