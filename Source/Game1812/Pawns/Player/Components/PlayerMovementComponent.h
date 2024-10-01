#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "../PlayerCameraStateEnum.h"

#include "PlayerMovementComponent.generated.h"


UCLASS()
class GAME1812_API UPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPlayerMovementComponent();

protected:

	class APlayerPawn* PlayerPawn;

	UPROPERTY(VisibleAnywhere)
	EPlayerCameraState MapState;

	UPROPERTY(VisibleAnywhere)
	TArray<class APlayerCameraSpot*> CameraSpots;

	UPROPERTY(VisibleAnywhere)
	int CurrentSpot;

	UPROPERTY(EditAnywhere, Category = "Camera Spots Movement")
	float LocationInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Camera Spots Movement")
	float RotationInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Map Movement")
	float MovementInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Map Movement")
	FVector2D TargetLocation;

	UPROPERTY(EditAnywhere, Category = "Map Movement")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Map Movement")
	float HalfWidthMapBorder;

	UPROPERTY(EditAnywhere, Category = "Map Movement")
	float HalfHeightMapBorder;

	UPROPERTY(EditAnywhere, Category = "Map Movement")
	float GlobalMapArmLength;

	UPROPERTY(EditAnywhere, Category = "Map Movement")
	float HalfWidthGlobalMapBorder;

	UPROPERTY(EditAnywhere, Category = "Map Movement")
	float HalfHeightGlobalMapBorder;

	virtual void BeginPlay() override;

	void FindCameraSpots(TArray<class APlayerCameraSpot*>& Spots);
	int GetDefaultSpot();

	void UpdateCameraSpot();

	FVector2D ApplyBoundsToPoint(const FVector2D& Point);

	void MoveCameraToCurrentSpot(float DeltaTime);
	void MoveCameraToMap(float DeltaTime);

	void UpdateMovementOnMap(float DeltaTime);

	FVector2D GetInputDirection();

public:

	EPlayerCameraState GetMapState();

	float GetGlobalMapArmLength();
	bool IsInGlobalMapBounds();
	bool IsInGlobalMap();

	void ChangeCameraSpot(int deltaIndex);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};