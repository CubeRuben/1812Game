#include "UnitMovementComponent.h"

#include <NavigationPath.h>
#include <NavigationSystem.h>

#include "../BaseUnit.h"

UUnitMovementComponent::UUnitMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TargetLocation = FVector::ZeroVector;
	bIsMoving = false;
	bMustRotateToTargetRotation = false;

	Path = nullptr;
	CurrentFollowingSegmentIndex = 0;
	LastTimeOfMoveAssign = -100.f;
}

void UUnitMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	UnitPawn = Cast<ABaseUnit>(GetOwner());

	if (!UnitPawn) 
		DestroyComponent();

	TargetLocation = UnitPawn->GetActorLocation();
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsMoving) 
	{
		if (bMustRotateToTargetRotation) 
		{
			const float rotation = (FRotator(0.f, TargetRotation, 0.f) - UnitPawn->GetActorRotation()).Yaw;

			RotatePawn(DeltaTime, rotation);
		}

		return;
	}
	
	MoveAlongPath(DeltaTime);
}

void UUnitMovementComponent::MoveAlongPath(float DeltaTime)
{
	if (!Path || !Path->IsValid())
		return;

	const FVector startLocation = UnitPawn->GetActorLocation();

	UpdateMovement(DeltaTime);

	//Check if bound, because sqrt is costly operation
	if (OnMove.IsBound()) 
	{
		const float movedDistance = FVector::DistSquared2D(startLocation, UnitPawn->GetActorLocation());
		OnMove.Broadcast(movedDistance);
	}

	CheckMovementEnd();
}

void UUnitMovementComponent::UpdateMovement(float DeltaTime) 
{
	const FVector targetPoint = GetNextPathPoint();

	const FVector movementDirection = (targetPoint - UnitPawn->GetActorLocation()).GetSafeNormal2D();
	const float rotationYaw = FQuat::FindBetween(UnitPawn->GetActorForwardVector(), movementDirection).Rotator().Yaw;

	RotatePawn(DeltaTime, rotationYaw);

	if (FMath::Abs(rotationYaw) < 5.0f)
		MovePawn(DeltaTime, targetPoint);
}

void UUnitMovementComponent::MovePawn(float DeltaTime, const FVector& Location)
{
	const FVector delta = (Location - UnitPawn->GetActorLocation()) * FVector(1, 1, 0);
	const FVector direction = delta.GetSafeNormal();
	const FVector movementDelta = direction * UnitPawn->GetMovementSpeed() * DeltaTime;

	if (movementDelta.SizeSquared() > delta.SizeSquared())
	{
		UnitPawn->SetActorLocation(Location);
	}
	else
	{
		UnitPawn->AddActorWorldOffset(movementDelta);
	}

	UnitPawn->AddActorWorldOffset(FVector(0, 0, 50));
	UnitPawn->AddActorWorldOffset(FVector(0, 0, -100), true);
}

void UUnitMovementComponent::RotatePawn(float DeltaTime, float RotationYaw)
{
	const float rotationDelta = FMath::Sign(RotationYaw) * DeltaTime * UnitPawn->GetRotationSpeed();
	const float limitedRotation = FMath::Clamp(rotationDelta, -FMath::Abs(RotationYaw), FMath::Abs(RotationYaw));

	UnitPawn->AddActorLocalRotation(FRotator(0, limitedRotation, 0));
}

void UUnitMovementComponent::MoveTo(const FVector& MoveToLocation, bool bForceMove)
{
	if (LastTimeOfMoveAssign + 1.f > GetWorld()->GetTimeSeconds() && !bForceMove)
		return;

	bMustRotateToTargetRotation = false;
	FVector moveToLocation = ProjectPointToMap(MoveToLocation);

	if (TargetLocation == moveToLocation)
		return OnMovementEnd.Broadcast();

	TargetLocation = moveToLocation;

	CheckMovementStart();

	if (bIsMoving)
	{
		UpdatePath();
		LastTimeOfMoveAssign = GetWorld()->GetTimeSeconds();
	}
	else
	{
		OnMovementEnd.Broadcast();
	}
}

void UUnitMovementComponent::RotateTo(float FinishRotation)
{
	TargetRotation = FinishRotation;
	bMustRotateToTargetRotation = true;
}

void UUnitMovementComponent::StopMoving()
{
	TargetLocation = ProjectPointToMap(UnitPawn->GetActorLocation());

	if (bIsMoving)
		OnMovementEnd.Broadcast();

	bIsMoving = false;
}

FVector UUnitMovementComponent::ProjectPointToMap(const FVector& Point)
{
	FHitResult hit;

	GetWorld()->LineTraceSingleByChannel(hit, Point, Point - FVector(0, 0, 4000.f), ECollisionChannel::ECC_GameTraceChannel1);

	if (!hit.bBlockingHit)
		return Point;

	return hit.Location;
}

void UUnitMovementComponent::UpdatePath()
{
	CurrentFollowingSegmentIndex = 0;
	Path = UNavigationSystemV1::FindPathToLocationSynchronously(UnitPawn, UnitPawn->GetActorLocation(), TargetLocation, UnitPawn);
}

void UUnitMovementComponent::CheckMovementStart()
{
	if (FVector::DistSquaredXY(TargetLocation, UnitPawn->GetActorLocation()) < 5.f)
		return;

	bIsMoving = true;
	OnMovementStart.Broadcast();
}

void UUnitMovementComponent::CheckMovementEnd()
{
	if (FVector::DistSquaredXY(GetLastPathPoint(), UnitPawn->GetActorLocation()) > 5.f)
		return;

	bIsMoving = false;
	OnMovementEnd.Broadcast();
}

FVector UUnitMovementComponent::GetNextPathPoint()
{
	if (!Path || Path->PathPoints.IsEmpty())
		return UnitPawn->GetActorLocation();

	if (!Path->PathPoints.IsValidIndex(CurrentFollowingSegmentIndex))
	{
		CurrentFollowingSegmentIndex = 0;
		return UnitPawn->GetActorLocation();
	}

	if (FVector::DistSquaredXY(Path->PathPoints[CurrentFollowingSegmentIndex], UnitPawn->GetActorLocation()) < 10.f)
	{
		if ((Path->PathPoints.Num() - 1) != CurrentFollowingSegmentIndex)
			CurrentFollowingSegmentIndex++;
	}

	return Path->PathPoints[CurrentFollowingSegmentIndex];
}

FVector UUnitMovementComponent::GetLastPathPoint() 
{
	if (!Path || Path->PathPoints.IsEmpty())
		return UnitPawn->GetActorLocation();

	return Path->PathPoints.Last();
}

FVector UUnitMovementComponent::GetTargetLocation() const
{
	return TargetLocation;
}

bool UUnitMovementComponent::IsMoving()
{
	return bIsMoving;
}