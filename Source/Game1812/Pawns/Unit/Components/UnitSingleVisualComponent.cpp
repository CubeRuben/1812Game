#include "UnitSingleVisualComponent.h"

#include "UnitVisualMeshComponent.h"
#include "../BaseUnit.h"
#include "../../../Actors/GlobalUnitCombatVisual.h"

UUnitSingleVisualComponent::UUnitSingleVisualComponent()
{

}

void UUnitSingleVisualComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UnitPawn = Cast<ABaseUnit>(GetOwner());

	if (!UnitPawn)
	{
		DestroyComponent();
		return;
	}

	Init();
}

void UUnitSingleVisualComponent::UpdateVisual(float DeltaTime)
{
	if (!VisualMeshComponent)
		return;

	const FVector rootLocation = GetOwner()->GetRootComponent()->GetComponentLocation();
	const float rootRotation = GetOwner()->GetRootComponent()->GetComponentRotation().Yaw;

	const float movementSpeed = UnitPawn->GetMovementSpeed() * 1.25f;
	const float rotationSpeed = UnitPawn->GetRotationSpeed() * 1.25f;
	const float teleportDistance = 10000.0f;

	const FVector componentLocation = VisualMeshComponent->GetComponentLocation();
	const FVector targetMovement = rootLocation - componentLocation;
	const float deltaMove = movementSpeed * DeltaTime;

	const float targetMovementSizeSquared = targetMovement.SizeSquared2D();

	const bool atTarget = deltaMove * deltaMove >= targetMovementSizeSquared;
	const bool isFurtherForceTeleportDistance = teleportDistance <= targetMovementSizeSquared;

	float targetDeltaRotation = 0.0f;

	if (atTarget || isFurtherForceTeleportDistance)
	{
		VisualMeshComponent->SetWorldLocation(rootLocation);

		targetDeltaRotation = FQuat::FindBetween(VisualMeshComponent->GetForwardVector(), GetOwner()->GetActorForwardVector()).Rotator().Yaw;
	}
	else
	{
		const FVector movementDirection = targetMovement.GetSafeNormal2D();
		const FVector movementDelta = movementDirection * deltaMove;

		VisualMeshComponent->AddWorldOffset(movementDelta);

		targetDeltaRotation = FQuat::FindBetween(VisualMeshComponent->GetForwardVector(), movementDirection).Rotator().Yaw;
	}

	if (FMath::Abs(targetDeltaRotation) >= 0.1f)
	{
		const float deltaRotation = rotationSpeed * DeltaTime;

		VisualMeshComponent->AddWorldRotation(FRotator(0.0f, deltaRotation * FMath::Sign(targetDeltaRotation), 0.0f));
	}
}

void UUnitSingleVisualComponent::Init()
{
	VisualMeshComponent = CreateVisualComponent("Visual Mesh Component", VisualMesh);

	if (!VisualMeshComponent)
		return;

	const FVector rootLocation = GetOwner()->GetRootComponent()->GetComponentLocation();
	const float rootRotation = GetOwner()->GetRootComponent()->GetComponentRotation().Yaw;

	VisualMeshComponent->SetWorldLocation(rootLocation);
	VisualMeshComponent->SetWorldRotation(FRotator(0.0f, rootRotation, 0.0f));

	AGlobalUnitCombatVisual::GetInstance()->AddVisualComponent(this, VisualMeshComponent);
}
