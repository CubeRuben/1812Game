#include "UnitMovementComponent.h"

#include <NavigationPath.h>
#include <NavigationSystem.h>

#include "../Orders/FormationMovement.h"
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
	const FVector actorLocation = UnitPawn->GetActorLocation();

	const FVector delta = (Location - actorLocation) * FVector(1, 1, 0);
	const FVector direction = delta.GetSafeNormal();

	float movementSpeed = UnitPawn->GetMovementSpeed();

	if (FormationMovement) 
	{
		movementSpeed = FMath::Min(movementSpeed, FormationMovement->GetMovementSpeed());
	}

	const FVector movementDelta = direction * movementSpeed * DeltaTime;

	FVector newActorLocation;

	if (movementDelta.SizeSquared2D() > delta.SizeSquared2D())
	{
		newActorLocation = Location;
	}
	else
	{
		newActorLocation = actorLocation + movementDelta;
	}

	FHitResult hit;

	GetWorld()->LineTraceSingleByChannel(hit, newActorLocation + FVector(0.0f, 0.0f, 250.f), newActorLocation - FVector(0.0f, 0.0f, 250.f), ECollisionChannel::ECC_GameTraceChannel1);

	if (hit.bBlockingHit)
	{
		newActorLocation = hit.Location;
	}

	UnitPawn->SetActorLocation(newActorLocation);
}

void UUnitMovementComponent::RotatePawn(float DeltaTime, float RotationYaw)
{
	float rotationSpeed = UnitPawn->GetRotationSpeed();

	if (FormationMovement)
	{
		if (rotationSpeed > FormationMovement->GetRotationSpeed())
			rotationSpeed = FormationMovement->GetRotationSpeed();
	}

	const float rotationDelta = FMath::Sign(RotationYaw) * DeltaTime * rotationSpeed;
	const float limitedRotation = FMath::Clamp(rotationDelta, -FMath::Abs(RotationYaw), FMath::Abs(RotationYaw));

	UnitPawn->AddActorLocalRotation(FRotator(0, limitedRotation, 0));
}

void UUnitMovementComponent::MoveTo(const FVector& MoveToLocation, EUnitMovementType MovementType)
{
	if (LastTimeOfMoveAssign + 1.f > GetWorld()->GetTimeSeconds())
		return;

	ForceMoveTo(MoveToLocation, MovementType);
}

void UUnitMovementComponent::ForceMoveTo(const FVector& MoveToLocation, EUnitMovementType MovementType)
{
	if (MovementType == EUnitMovementType::Attack) 
	{
		if (FormationMovement)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "UUnitMovementComponent::ForceMoveTo() - Clear");

		FormationMovement = nullptr;
	}
		

	bMustRotateToTargetRotation = false;
	FVector moveToLocation = ProjectPointToMap(MoveToLocation);

	if (TargetLocation == moveToLocation) 
	{
		OnMovementEnd.Broadcast();
		OnMovementEndGlobalEvent.Broadcast(UnitPawn);
		return;
	}

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
		OnMovementEndGlobalEvent.Broadcast(UnitPawn);
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
	{
		OnMovementEnd.Broadcast();
		OnMovementEndGlobalEvent.Broadcast(UnitPawn);
	}

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
	OnMovementEndGlobalEvent.Broadcast(UnitPawn);
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

void UUnitMovementComponent::SetFormationMovement(UFormationMovement* NewFormationMovement)
{
	FormationMovement = NewFormationMovement;
}

bool UUnitMovementComponent::IsMoving()
{
	return bIsMoving;
}