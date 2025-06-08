#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PocketWatch.generated.h"

UCLASS()
class GAME1812_API APocketWatch : public AActor
{
	GENERATED_BODY()
	
public:	

	APocketWatch();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* WatchMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ArrowSecMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ArrowMinMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ArrowHourMeshComponent;

	class ACossacksGameMode* GameMode;

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};
