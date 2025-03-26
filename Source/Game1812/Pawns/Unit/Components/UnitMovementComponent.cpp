#include "UnitMovementComponent.h"

#include <NavigationPath.h>
#include <NavigationSystem.h>

#include "../Orders/FormationMovement.h"
#include "../BaseUnit.h"

FOnMovementEndStaticDelegate UUnitMovementComponent::OnMovementEndGlobalEvent = FOnMovementEndStaticDelegate();

UUnitMovementComponent::UUnitMovementComponent() :
	UnitPawn(nullptr), TargetLocation(FVector::ZeroVector), TargetRotation(0.0f)
{
	PrimaryComponentTick.bCanEverTick = true;

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
	TargetRotation = UnitPawn->GetActorRotation().Yaw;
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsMoving)
	{
		MoveAlongPath(DeltaTime);
		return;
	}

	if (bMustRotateToTargetRotation) 
	{
		const float rotation = (FRotator(0.0f, TargetRotation, 0.0f) - UnitPawn->GetActorRotation()).Yaw;
		RotatePawn(DeltaTime, rotation);
	}
}

void UUnitMovementComponent::MoveAlongPath(float DeltaTime)
{
	if (!Path || !Path->IsValid()) 
	{
		StopMoving();
		return;
	}

	const float deltaDistance = UpdateMovement(DeltaTime);

	OnMove.Broadcast(deltaDistance);

	CheckMovementEnd();
}

float UUnitMovementComponent::UpdateMovement(float DeltaTime) 
{
	FVector targetPoint = GetNextPathPoint();

	const FVector movementDirection = (targetPoint - UnitPawn->GetActorLocation()).GetSafeNormal2D();
	
	const float rotationYaw = FQuat::FindBetween(UnitPawn->GetActorForwardVector(), movementDirection).Rotator().Yaw;
	RotatePawn(DeltaTime, rotationYaw);

	if (FMath::Abs(rotationYaw) > 90.0f)
		return 0.0f;

	const float deltaDistance = GetMovementSpeed() * DeltaTime;
	float leftDeltaDistance = deltaDistance;

	const FVector actorLocation = UnitPawn->GetActorLocation();
	FVector newActorLocation;

	for (int i = 0; i < 25; i++)
	{
		const FVector delta = targetPoint - actorLocation;
		const FVector direction = delta.GetSafeNormal2D();
		const FVector movementDelta = direction * deltaDistance;

		if (movementDelta.SizeSquared2D() > delta.SizeSquared2D())
		{
			newActorLocation = targetPoint;
			leftDeltaDistance -= delta.Size2D();
		}
		else
		{
			newActorLocation = actorLocation + movementDelta;
			leftDeltaDistance = 0.0f;
			break;
		}

		targetPoint = GetNextPathPoint(newActorLocation);

		if ((targetPoint - newActorLocation).IsNearlyZero())
			break;
	}

	FHitResult hit;

	const FVector startPoint = newActorLocation + FVector(0.0f, 0.0f, 250.f);
	const FVector endPoint = newActorLocation - FVector(0.0f, 0.0f, 250.f);

	GetWorld()->LineTraceSingleByChannel(hit, startPoint, endPoint, ECollisionChannel::ECC_GameTraceChannel1);

	if (hit.bBlockingHit)
		newActorLocation = hit.Location;

	UnitPawn->SetActorLocation(newActorLocation);

	return deltaDistance - leftDeltaDistance;
}

void UUnitMovementComponent::RotatePawn(float DeltaTime, float RotationYaw)
{
	const float rotationSpeed = GetRotationSpeed();
	const float rotationDelta = FMath::Sign(RotationYaw) * DeltaTime * rotationSpeed;
	const float limitedRotation = FMath::Clamp(rotationDelta, -FMath::Abs(RotationYaw), FMath::Abs(RotationYaw));

	UnitPawn->AddActorLocalRotation(FRotator(0.0f, limitedRotation, 0.0f));
}

void UUnitMovementComponent::MoveTo(const FVector& MoveToLocation, EUnitMovementType MovementType)
{
	const float cooldownTime = LastTimeOfMoveAssign + 1.0f;
	const float worldTime = GetWorld()->GetTimeSeconds();

	if (cooldownTime > worldTime)
		return;

	ForceMoveTo(MoveToLocation, MovementType);
}

void UUnitMovementComponent::ForceMoveTo(const FVector& MoveToLocation, EUnitMovementType MovementType)
{
	if (MovementType == EUnitMovementType::Attack) 
		FormationMovement = nullptr;

	bMustRotateToTargetRotation = false;
	TargetLocation = ProjectPointToMap(MoveToLocation);
	LastTimeOfMoveAssign = GetWorld()->GetTimeSeconds();

	CheckMovementStart();

	if (bIsMoving)
	{
		UpdatePath();
		LastTimeOfMoveAssign = GetWorld()->GetTimeSeconds();
	}
	else 
	{
		Path = nullptr;
		OnMovementEnd.Broadcast();
	}
}

void UUnitMovementComponent::MoveToActor(AActor* Actor)
{
	if (!Actor)
		return;

	TargetActor = Actor;

	ForceMoveTo(Actor->GetActorLocation());

	if (bIsMoving)
		GetWorld()->GetTimerManager().SetTimer(UpdatePathTimerHandle, this, &UUnitMovementComponent::UpdatePathToActor, 1.0f, true, 1.0f);
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

	GetWorld()->GetTimerManager().ClearTimer(UpdatePathTimerHandle);
	bIsMoving = false;
}

FVector UUnitMovementComponent::ProjectPointToMap(const FVector& Point)
{
	FHitResult hit;

	const FVector startPoint = Point + FVector(0.0f, 0.0f, 1000.0f);
	const FVector endPoint = Point - FVector(0.0f, 0.0f, 4000.0f);

	GetWorld()->LineTraceSingleByChannel(hit, startPoint, endPoint, ECollisionChannel::ECC_GameTraceChannel1);

	if (hit.bBlockingHit)
		return hit.Location;

	return Point;
}

void UUnitMovementComponent::UpdatePath()
{
	CurrentFollowingSegmentIndex = 0;
	Path = UNavigationSystemV1::FindPathToLocationSynchronously(UnitPawn, UnitPawn->GetActorLocation(), TargetLocation, UnitPawn);

	if (Path)
		Path->EnableDebugDrawing(true);
}

void UUnitMovementComponent::UpdatePathToActor()
{
	if (!TargetActor) 
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdatePathTimerHandle);
		return;
	}

	TargetLocation = TargetActor->GetActorLocation();
	UpdatePath();
}

void UUnitMovementComponent::CheckMovementStart()
{
	if (FVector::DistSquared2D(TargetLocation, UnitPawn->GetActorLocation()) < 7.5f)
		return;

	bIsMoving = true;
	OnMovementStart.Broadcast();
}

void UUnitMovementComponent::CheckMovementEnd()
{
	if (FVector::DistSquared2D(GetLastPathPoint(), UnitPawn->GetActorLocation()) > 5.0f)
		return;

	bIsMoving = false;
	GetWorld()->GetTimerManager().ClearTimer(UpdatePathTimerHandle);
	OnMovementEnd.Broadcast();
	OnMovementEndGlobalEvent.Broadcast(UnitPawn);
}

float UUnitMovementComponent::GetMovementSpeed()
{
	const float unitMovementSpeed = UnitPawn->GetMovementSpeed();

	if (FormationMovement)
		return FMath::Min(unitMovementSpeed, FormationMovement->GetMovementSpeed());

	return unitMovementSpeed;
}

float UUnitMovementComponent::GetRotationSpeed()
{
	const float unitRotationSpeed = UnitPawn->GetRotationSpeed();

	if (FormationMovement)
		return FMath::Min(unitRotationSpeed, FormationMovement->GetMovementSpeed());

	return unitRotationSpeed;
}

FVector UUnitMovementComponent::GetNextPathPoint()
{
	return GetNextPathPoint(UnitPawn->GetActorLocation());
}

FVector UUnitMovementComponent::GetNextPathPoint(const FVector& ActorLocation)
{
	if (!Path || Path->PathPoints.IsEmpty())
		return ActorLocation;

	if (!Path->PathPoints.IsValidIndex(CurrentFollowingSegmentIndex))
	{
		CurrentFollowingSegmentIndex = 0;
		return ActorLocation;
	}

	if (FVector::DistSquaredXY(Path->PathPoints[CurrentFollowingSegmentIndex], ActorLocation) < 10.f)
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

bool UUnitMovementComponent::IsMoving() const
{
	return bIsMoving;
}