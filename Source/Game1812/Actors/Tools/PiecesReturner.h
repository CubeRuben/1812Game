#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PiecesReturner.generated.h"

UCLASS()
class GAME1812_API APiecesReturner : public AActor
{
	GENERATED_BODY()
	
public:	

	APiecesReturner();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* ReturnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RandomOffsetRadius;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
