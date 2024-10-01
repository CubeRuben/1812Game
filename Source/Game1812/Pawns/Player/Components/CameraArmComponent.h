#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CameraArmComponent.generated.h"

UCLASS()
class GAME1812_API UCameraArmComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UCameraArmComponent();

protected:

	class APlayerPawn* PlayerPawn;

	UPROPERTY(EditDefaultsOnly)
	float CurrentLength;

	UPROPERTY(VisibleAnywhere)
	float TargetLength;

	UPROPERTY(EditAnywhere)
	float MaxLength;

	UPROPERTY(EditAnywhere)
	float MinLength;

	UPROPERTY(EditAnywhere)
	float Interpolation;

	virtual void BeginPlay() override;

public:

	float GetCurrentLength() { return CurrentLength; }
	float GetHeight() { return FMath::Abs(CurrentLength * FMath::Sin(FMath::DegreesToRadians(GetRelativeRotation().Pitch))); }

	float AddTargetLength(float deltaLength);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
