#include "UnitCombatVisualComponent.h"

#include "../Units/CombatUnit.h"
#include "../../../DataAssets/CombatUnitDataAsset.h"
#include "../../../Actors/GlobalUnitCombatVisual.h"

#include <Components/StaticMeshComponent.h>

UUnitCombatVisualComponent::UUnitCombatVisualComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUnitCombatVisualComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatUnitPawn = Cast<ACombatUnit>(GetOwner());

	if (!CombatUnitPawn) 
	{
		DestroyComponent();
		return;
	}

	if (!AGlobalUnitCombatVisual::GetInstance()) 
		AGlobalUnitCombatVisual::CreateInstance(GetWorld());
}

void UUnitCombatVisualComponent::UpdateVisual(float DeltaTime)
{
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

		float targetDeltaRotation = 0.0f;

		if (atTarget)
		{
			component->SetWorldLocation(targetLocation);

			targetDeltaRotation = FQuat::FindBetween(component->GetForwardVector(), GetOwner()->GetActorForwardVector()).Rotator().Yaw;
		}
		else 
		{
			const FVector movementDirection = targetMovement.GetSafeNormal2D();
			const FVector movementDelta = movementDirection * deltaMove;

			component->AddWorldOffset(movementDelta);

			targetDeltaRotation = FQuat::FindBetween(component->GetForwardVector(), movementDirection).Rotator().Yaw;
		}

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

	if (!AGlobalUnitCombatVisual::GetInstance())
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

	UpdateFormationOffsets(meshesNumber, combatUnitVisual.GetMeshOffset());

	const FVector rootLocation = GetOwner()->GetRootComponent()->GetComponentLocation();
	const float rootRotation = GetOwner()->GetRootComponent()->GetComponentRotation().Yaw;

	for (int i = 0; i < meshesNumber; i++) 
	{
		const FVector rotatedOffset = MeshesOffsets[i].RotateAngleAxis(rootRotation, FVector::UpVector);
		const FVector location = rotatedOffset + rootLocation;

		UnitMeshComponents[i]->SetWorldLocation(location);
		UnitMeshComponents[i]->SetWorldRotation(FRotator(0.0f, rootRotation, 0.0f));
	}

	AGlobalUnitCombatVisual::GetInstance()->AddVisualComponents(this, UnitMeshComponents);
}

void UUnitCombatVisualComponent::UpdateFormationOffsets(int Number, float Offset)
{
	MeshesOffsets.Empty();
	MeshesOffsets.SetNum(Number);

	const int firstRow = FMath::RoundToInt((float)Number / 2.0f) - (Number > 3);

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