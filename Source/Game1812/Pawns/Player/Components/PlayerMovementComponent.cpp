#include "PlayerMovementComponent.h"

#include "../PlayerPawn.h"
#include "CameraArmComponent.h"
#include "../../../Actors/Tools/PlayerCameraSpot.h"

#include <Kismet/GameplayStatics.h>
#include <Camera/CameraComponent.h>


UPlayerMovementComponent::UPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MapState = EPlayerCameraState::OutOfMap;

	HalfWidthMapBorder = 2400.0f;
	HalfHeightMapBorder = 1600.0f;
	HalfWidthGlobalMapBorder = 1500.0f;
	HalfHeightGlobalMapBorder = 1500.0f;
	GlobalMapArmLength = 450.0f;
	Speed = 1600.0f;

	LocationInterpSpeed = 15.0f;
	RotationInterpSpeed = 20.0f;

	MovementInterpSpeed = 5.0f;
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner())
		return DestroyComponent();

	PlayerPawn = Cast<APlayerPawn>(GetOwner());

	if (!PlayerPawn)
		return DestroyComponent();


	FindCameraSpots(CameraSpots);

	CurrentSpot = GetDefaultSpot();

	if (CameraSpots.Num() != 0)
		PlayerPawn->GetCameraComponent()->SetWorldTransform(CameraSpots[CurrentSpot]->GetActorTransform());
}

void UPlayerMovementComponent::FindCameraSpots(TArray<class APlayerCameraSpot*>& Spots)
{
	TArray<AActor*> spotActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCameraSpot::StaticClass(), spotActors);

	TArray<APlayerCameraSpot*> spots;

	for (auto spot : spotActors)
	{
		APlayerCameraSpot* cameraSpot = Cast<APlayerCameraSpot>(spot);

		if (!cameraSpot)
			continue;

		spots.Add(cameraSpot);
	}

	spots.Sort();

	for (auto spot : spots)
	{
		Spots.Add(spot);
	}
}

int UPlayerMovementComponent::GetDefaultSpot()
{
	for (int i = 0; i < CameraSpots.Num(); i++)
	{
		if (CameraSpots[i]->GetPriority() == 0)
			return i;
	}

	return 0;
}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MapState != EPlayerCameraState::OutOfMap) 
	{
		if (PlayerPawn->GetPlayerInput()->LookAtMap)
		{
			MapState = EPlayerCameraState::OutOfMap;
			PlayerPawn->GetPlayerInput()->LookAtMap = false;

			PlayerPawn->GetPlayerInput()->MoveLeft = false;
			PlayerPawn->GetPlayerInput()->MoveRight = false;
		}
		else 
		{
			if (MapState == EPlayerCameraState::LookingAtMap) 
			{
				UpdateMovementOnMap(DeltaTime);
			}
			else 
			{
				MoveCameraToMap(DeltaTime);
			}
		}
	}
	else 
	{
		if (PlayerPawn->GetPlayerInput()->LookAtMap || PlayerPawn->GetPlayerInput()->MoveForward)
		{
			MapState = EPlayerCameraState::MovingToMap;
			PlayerPawn->GetPlayerInput()->LookAtMap = false;
			PlayerPawn->GetPlayerInput()->MoveForward = false;
		}
		else
		{
			UpdateCameraSpot();
			MoveCameraToCurrentSpot(DeltaTime);
		}
	}
}

void UPlayerMovementComponent::UpdateCameraSpot()
{
	if (PlayerPawn->GetPlayerInput()->MoveLeft)
	{
		ChangeCameraSpot(-1);

		PlayerPawn->GetPlayerInput()->MoveLeft = false;
	}

	if (PlayerPawn->GetPlayerInput()->MoveRight)
	{
		ChangeCameraSpot(1);

		PlayerPawn->GetPlayerInput()->MoveRight = false;
	}
}

FVector2D UPlayerMovementComponent::ApplyBoundsToPoint(const FVector2D& Point)
{
	const bool isOnGlobalMap = IsInGlobalMap();

	const float halfHeight = isOnGlobalMap ? HalfHeightGlobalMapBorder : HalfHeightMapBorder;
	const float halfWidth = isOnGlobalMap ? HalfWidthGlobalMapBorder : HalfWidthMapBorder;

	return FVector2D(FMath::Clamp(TargetLocation.X, -halfHeight, halfHeight), FMath::Clamp(TargetLocation.Y, -halfWidth, halfWidth));
}

void UPlayerMovementComponent::UpdateMovementOnMap(float DeltaTime)
{
	const FVector2D inputDirection = GetInputDirection();
	TargetLocation += inputDirection * Speed * DeltaTime;

	TargetLocation = ApplyBoundsToPoint(TargetLocation);

	const float ScrollDelta = PlayerPawn->GetCameraArmComponent()->AddTargetLength(-PlayerPawn->GetPlayerInput()->MouseScroll * 120);

	if (ScrollDelta != 0.0f)
	{
		FVector cursorLocation, cursorDirection;
		PlayerPawn->GetLocalViewingPlayerController()->DeprojectMousePositionToWorld(cursorLocation, cursorDirection);

		const float Height = PlayerPawn->GetCameraArmComponent()->GetHeight();
		const float HeightRatio = cursorDirection.Z / Height * -ScrollDelta;
		const FVector LookingPoint = FMath::RayPlaneIntersection(PlayerPawn->GetCameraComponent()->GetComponentLocation(), cursorDirection, FPlane(PlayerPawn->GetCameraArmComponent()->GetComponentLocation(), FVector::UpVector));
		FVector delta = LookingPoint - PlayerPawn->GetCameraArmComponent()->GetComponentLocation();
		delta *= HeightRatio;
		TargetLocation += FVector2D(delta);
	}

	PlayerPawn->GetPlayerInput()->MouseScroll = 0;

	const FVector2D NewLocation = FMath::Vector2DInterpTo(FVector2D(PlayerPawn->GetCameraArmComponent()->GetRelativeLocation()), TargetLocation, DeltaTime, MovementInterpSpeed);
	PlayerPawn->GetCameraArmComponent()->SetRelativeLocation(FVector(NewLocation, PlayerPawn->GetCameraArmComponent()->GetRelativeLocation().Z));
}

void UPlayerMovementComponent::MoveCameraToCurrentSpot(float DeltaTime) 
{
	APlayerCameraSpot* spot = CameraSpots[CurrentSpot];

	float lip = LocationInterpSpeed;
	float rip = RotationInterpSpeed;

	if (spot->HaveCustomLocationInterpSpeed()) 
		lip = spot->GetLocationInterpSpeed();

	if (spot->HaveCustomRotationInterpSpeed()) 
		rip = spot->GetRotationInterpSpeed();

	const FVector moveDelta = FMath::VInterpTo(PlayerPawn->GetCameraComponent()->GetComponentLocation(), spot->GetActorLocation(), DeltaTime, lip);
	const FRotator rotationDelta = FMath::RInterpTo(PlayerPawn->GetCameraComponent()->GetComponentRotation(), spot->GetActorRotation(), DeltaTime, rip);

	PlayerPawn->GetCameraComponent()->SetWorldLocation(moveDelta);
	PlayerPawn->GetCameraComponent()->SetWorldRotation(rotationDelta);
}

void UPlayerMovementComponent::MoveCameraToMap(float DeltaTime)
{
	USceneComponent* cameraPoint = PlayerPawn->GetCameraArmPoint();

	const FVector moveDelta = FMath::VInterpTo(PlayerPawn->GetCameraComponent()->GetComponentLocation(), cameraPoint->GetComponentLocation(), DeltaTime, LocationInterpSpeed);
	const FRotator rotationDelta = FMath::RInterpTo(PlayerPawn->GetCameraComponent()->GetComponentRotation(), cameraPoint->GetComponentRotation(), DeltaTime, RotationInterpSpeed);

	PlayerPawn->GetCameraComponent()->SetWorldLocation(moveDelta);
	PlayerPawn->GetCameraComponent()->SetWorldRotation(rotationDelta);

	if ((PlayerPawn->GetCameraComponent()->GetComponentLocation() - cameraPoint->GetComponentLocation()).IsNearlyZero(1) &&
		(PlayerPawn->GetCameraComponent()->GetComponentRotation() - cameraPoint->GetComponentRotation()).IsNearlyZero(0.05))
	{
		MapState = EPlayerCameraState::LookingAtMap;

		PlayerPawn->GetCameraComponent()->SetWorldLocation(cameraPoint->GetComponentLocation());
		PlayerPawn->GetCameraComponent()->SetWorldRotation(cameraPoint->GetComponentRotation());
	}
}

FVector2D UPlayerMovementComponent::GetInputDirection()
{
	FVector direction(0);

	//Keyboard movement
	if (PlayerPawn->GetPlayerInput()->MoveForward) direction += PlayerPawn->GetActorForwardVector();
	if (PlayerPawn->GetPlayerInput()->MoveBack) direction -= PlayerPawn->GetActorForwardVector();
	if (PlayerPawn->GetPlayerInput()->MoveRight) direction += PlayerPawn->GetActorRightVector();
	if (PlayerPawn->GetPlayerInput()->MoveLeft) direction -= PlayerPawn->GetActorRightVector();

	APlayerController* playerController = PlayerPawn->GetController<APlayerController>();

	if (!playerController)
		return FVector2D(direction).GetSafeNormal();
	
	//Mouse window edge movement
	float cursorX, cursorY;

	if (playerController->GetMousePosition(cursorX, cursorY))
	{
		const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		const FVector2D	CursorRelativeLocation = FVector2D(cursorX, cursorY) / ViewportSize;

		const float MoveMargin = 0.01;

		if (CursorRelativeLocation.Y < MoveMargin) direction += PlayerPawn->GetActorForwardVector();
		if (CursorRelativeLocation.Y > 1 - MoveMargin) direction -= PlayerPawn->GetActorForwardVector();
		if (CursorRelativeLocation.X > 1 - MoveMargin) direction += PlayerPawn->GetActorRightVector();
		if (CursorRelativeLocation.X < MoveMargin) direction -= PlayerPawn->GetActorRightVector();
	}

	return FVector2D(direction).GetSafeNormal();
}

EPlayerCameraState UPlayerMovementComponent::GetMapState()
{
	return MapState;
}

float UPlayerMovementComponent::GetGlobalMapArmLength()
{
	return GlobalMapArmLength;
}

bool UPlayerMovementComponent::IsInGlobalMapBounds()
{
	const FVector2D location(PlayerPawn->GetCameraArmComponent()->GetRelativeLocation());

	return (location > FVector2D(-HalfHeightGlobalMapBorder, -HalfWidthGlobalMapBorder)) && (location < FVector2D(HalfHeightGlobalMapBorder, HalfWidthGlobalMapBorder));
}

bool UPlayerMovementComponent::IsInGlobalMap()
{
	return PlayerPawn->GetCameraArmComponent()->GetCurrentLength() < GlobalMapArmLength;
}

void UPlayerMovementComponent::ChangeCameraSpot(int deltaIndex)
{
	CurrentSpot = FMath::Clamp(CurrentSpot + deltaIndex, 0, CameraSpots.Num() - 1);
}