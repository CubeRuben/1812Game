#include "UnitCombatVisualComponent.h"

#include "../Units/CombatUnit.h"
#include "../../../DataAssets/CombatUnitDataAsset.h"

#include <Components/StaticMeshComponent.h>

UUnitCombatVisualComponent::UUnitCombatVisualComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUnitCombatVisualComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatUnitPawn = Cast<ACombatUnit>(GetOwner());

	if (!CombatUnitPawn)
		DestroyComponent();
}

void UUnitCombatVisualComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FVector rootLocation = GetOwner()->GetRootComponent()->GetComponentLocation();
	const float rootRotation = GetOwner()->GetRootComponent()->GetComponentRotation().Yaw;

	const FCombatUnitVisual& unitVisual = CombatUnitPawn->GetCombatUnitData()->GetCombatUnitVisual();

	for (int i = 0; i < UnitMeshComponents.Num(); i++)
	{
		UStaticMeshComponent* const component = UnitMeshComponents[i];

		const FVector rotatedOffset = MeshesOffsets[i].RotateAngleAxis(rootRotation, FVector::UpVector);
		const FVector targetLocation = rotatedOffset + rootLocation;

		const FVector componentLocation = component->GetComponentLocation();

		const FVector targetMovement = targetLocation - componentLocation;
		const float deltaMove = unitVisual.GetMovementSpeed() * DeltaTime;

		const bool atTarget = deltaMove * deltaMove >= targetMovement.SizeSquared2D();

		FVector newComponentLocation;
		float targetDeltaRotation = 0.0f;

		if (atTarget)
		{
			newComponentLocation = targetLocation;

			targetDeltaRotation = FQuat::FindBetween(component->GetForwardVector(), GetOwner()->GetActorForwardVector()).Rotator().Yaw;
		}
		else 
		{
			const FVector movementDirection = targetMovement.GetSafeNormal2D();
			const FVector movementDelta = movementDirection * deltaMove;

			newComponentLocation = componentLocation + movementDelta;

			targetDeltaRotation = FQuat::FindBetween(component->GetForwardVector(), movementDirection).Rotator().Yaw;
		}

		FHitResult hit;

		GetWorld()->LineTraceSingleByChannel(hit, newComponentLocation + FVector(0.0f, 0.0f, 250.f), newComponentLocation - FVector(0.0f, 0.0f, 250.f), ECollisionChannel::ECC_GameTraceChannel1);

		if (hit.bBlockingHit) 
		{
			newComponentLocation = hit.Location;
		}

		component->SetWorldLocation(newComponentLocation);

		if (FMath::Abs(targetDeltaRotation) >= 0.1f) 
		{
			const float deltaRotation = unitVisual.GetRotationSpeed() * DeltaTime;

			component->AddWorldRotation(FRotator(0.0f, deltaRotation * FMath::Sign(targetDeltaRotation), 0.0f));
		}
	}
}

void UUnitCombatVisualComponent::Init(UCombatUnitDataAsset* UnitCombatStats)
{
	if (!UnitCombatStats)
		return;

	const FCombatUnitVisual& combatUnitVisual = UnitCombatStats->GetCombatUnitVisual();
	const FCombatUnitStats& combatUnitStats = UnitCombatStats->GetCombatUnitStats();

	const int meshesNumber = FMath::RoundToInt(combatUnitStats.GetBaseHP() / combatUnitVisual.GetHealthPointsPerMesh());

	UnitMeshComponents.SetNum(meshesNumber);

	const FString componentBaseName("Combat Unit Mesh ");

	for (int i = 0; i < meshesNumber; i++) 
	{
		const FName componentName(componentBaseName + FString::FromInt(i));
		UStaticMeshComponent* component = NewObject<UStaticMeshComponent>(GetOwner(), UStaticMeshComponent::StaticClass(), componentName);
		
		if (!component)
			return;
		
		component->RegisterComponent();
		component->SetStaticMesh(combatUnitVisual.GetUnitMesh());
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		UnitMeshComponents[i] = component;
	}

	UpdateFormationOffsets(meshesNumber);

	const FVector rootLocation = GetOwner()->GetRootComponent()->GetComponentLocation();
	const float rootRotation = GetOwner()->GetRootComponent()->GetComponentRotation().Yaw;

	for (int i = 0; i < meshesNumber; i++) 
	{
		const FVector rotatedOffset = MeshesOffsets[i].RotateAngleAxis(rootRotation, FVector::UpVector);
		const FVector location = rotatedOffset + rootLocation;

		UnitMeshComponents[i]->SetWorldLocation(location);
		UnitMeshComponents[i]->SetWorldRotation(FRotator(0.0f, rootRotation, 0.0f));
	}
}

void UUnitCombatVisualComponent::UpdateFormationOffsets(int Number)
{
	const float Offset = 10.0f;

	MeshesOffsets.Empty();
	MeshesOffsets.SetNum(Number);

	const int firstRow = FMath::RoundToInt((float)Number / 2.0f) - (Number > 4);

	for (int i = 0; i < firstRow; i++)
	{
		const int mult = ((float)i / 2.0f + 1.0f);

		if (i % 2 == 0)
		{
			MeshesOffsets[i + 1] = FVector(0.0f, Offset * mult, 0.0f);
		}
		else
		{
			MeshesOffsets[i + 1] = FVector(0.0f, -Offset * mult, 0.0f);
		}
	}

	const float secondRowOffset = ((Number == 4) ? 0.0f : ((((2 * (firstRow + 1) - Number) % 2) == 0) ? 0.0f : (((((Number - 5) / 2) % 2) ? -1 : 1) * (Offset / 2.0f))));

	if (Number > 3)
	{
		MeshesOffsets[firstRow + 1] = FVector(-Offset, 0.0f - secondRowOffset, 0.0f);
	}

	for (int i = 0; i < Number - firstRow - 2; i++)
	{
		const int mult = ((float)i / 2.0f + 1.0f);

		if (i % 2 == 0)
		{
			MeshesOffsets[i + firstRow + 2] = FVector(-Offset, Offset * mult - secondRowOffset, 0.0f);
		}
		else
		{
			MeshesOffsets[i + firstRow + 2] = FVector(-Offset, -Offset * mult - secondRowOffset, 0.0f);
		}
	}
}