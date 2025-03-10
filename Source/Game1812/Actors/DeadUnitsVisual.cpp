#include "DeadUnitsVisual.h"

#include "Components/DeadUnitMeshComponent.h"
#include "../DataAssets/CombatUnitDataAsset.h"

ADeadUnitsVisual* ADeadUnitsVisual::Instance = nullptr;

ADeadUnitsVisual* ADeadUnitsVisual::GetInstance(UWorld* World)
{
	if (Instance)
		return Instance;

	Instance = World->SpawnActor<ADeadUnitsVisual>(ADeadUnitsVisual::StaticClass());

	return Instance;
}

ADeadUnitsVisual::ADeadUnitsVisual()
{
}

void ADeadUnitsVisual::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;
}

void ADeadUnitsVisual::BeginDestroy()
{
	Super::BeginDestroy();

	Instance = nullptr;
}

void ADeadUnitsVisual::AddDeadMesh(const FVector& Location, const FCombatUnitVisual& CombatUnitVisual)
{
	static const FString componentBaseName("Dead Unit Mesh ");
	
	const FName componentName(componentBaseName + FString::FromInt(DeadUnitMeshComponents.Num()));

	UDeadUnitMeshComponent* component = NewObject<UDeadUnitMeshComponent>(this, UDeadUnitMeshComponent::StaticClass(), componentName);

	if (!component)
		return;

	UStaticMesh* deadMesh = CombatUnitVisual.GetDeadUnitMeshOverride();

	if (!deadMesh)
		deadMesh = CombatUnitVisual.GetUnitMesh();

	component->RegisterComponent();
	component->SetStaticMesh(deadMesh);
	component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	component->SetWorldLocation(Location + CombatUnitVisual.GetDeadMeshOffset());
	component->SetWorldRotation(CombatUnitVisual.GetDeadMeshRotation());

	const FRotator randomRotationLimits = CombatUnitVisual.GetDeadMeshRandomRotation();
	const float randomRotationPitch = FMath::FRandRange(-randomRotationLimits.Pitch, randomRotationLimits.Pitch);
	const float randomRotationYaw = FMath::FRandRange(-randomRotationLimits.Yaw, randomRotationLimits.Yaw);
	const float randomRotationRoll = FMath::FRandRange(-randomRotationLimits.Roll, randomRotationLimits.Roll);
	const FRotator randomRotation(randomRotationPitch, randomRotationYaw, randomRotationRoll);

	component->AddWorldRotation(randomRotation);

	DeadUnitMeshComponents.Add(component);
	FogAffectedComponents.Add(component);
}
