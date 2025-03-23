#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConvoyPiece.generated.h"

UCLASS()
class GAME1812_API AConvoyPiece : public AActor
{
	GENERATED_BODY()

public:

	AConvoyPiece();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Height;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsCoveredInFog;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<class AEscortConvoyUnit> Unit;

	FVector GetTargetLocation();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCoveredInFogChange(bool IsCovered);

public:

	void SetOwnerUnit(class AEscortConvoyUnit* NewUnit);

	virtual void Tick(float DeltaTime) override;

	using AActor::GetTargetLocation;

};
