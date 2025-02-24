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
		const FVector rotatedOffset = MeshesOffsets[i].RotateAngleAxis(rootRotation, FVector::UpVector);
		const FVector targetLocation = rotatedOffset + rootLocation;

		const FVector componentLocation = UnitMeshComponents[i]->GetComponentLocation();

		const FVector movementDirection = (targetLocation - componentLocation).GetSafeNormal2D();
		const FVector movementDelta = movementDirection * unitVisual.GetMovementSpeed() * DeltaTime;

		UnitMeshComponents[i]->AddWorldOffset(movementDelta);
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