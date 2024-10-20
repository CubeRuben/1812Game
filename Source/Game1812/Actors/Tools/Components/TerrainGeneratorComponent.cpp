#include "TerrainGeneratorComponent.h"

#include "../TerrainPropertyVolume.h"
#include "../../../DataAssets/TerrainGeneratorDataAsset.h"
#include "PoissonPoints.h"

#include <Components/BoxComponent.h>

UTerrainGeneratorComponent::UTerrainGeneratorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UTerrainGeneratorComponent::OnRegister()
{
	Super::OnRegister();

	GenerateProps();
}

void UTerrainGeneratorComponent::GenerateProps()
{
	ClearProps();

	if (!OwnerVolume)
	{
		OwnerVolume = Cast<ATerrainPropertyVolume>(GetOwner());
		if (!OwnerVolume)
			return;
	}

	if (!GeneratorDataAsset)
		return;

	const TArray<UStaticMesh*>& meshVariants = GeneratorDataAsset->GetMeshVariants();

	if (meshVariants.Num() == 0)
		return;

	const bool bRandomRotation = GeneratorDataAsset->GetRandomRotation();

	const FVector actorLocation = OwnerVolume->GetActorLocation();
	const FRotator actorRotation = OwnerVolume->GetActorRotation();
	const FVector extent = OwnerVolume->GetBoxComponent()->GetScaledBoxExtent();

	FRandomStream randomStream(Seed);

	FPoissonPoints poissonPoints(randomStream, GeneratorDataAsset->GetMinDistance(), extent.X * 2.0f, extent.Y * 2.0f);
	poissonPoints.GeneratePoissonPoints();

	for (FVector2D& point : poissonPoints.GetPoints()) 
	{
		const FName propComponentName(FString("Prop Mesh Component ").Append(FString::FromInt(PropsMeshComponents.Num())));
		UStaticMeshComponent* propComponent = NewObject<UStaticMeshComponent>(OwnerVolume, UStaticMeshComponent::StaticClass(), propComponentName);

		const int randomMeshVariantIndex = randomStream.RandRange(0, meshVariants.Num() - 1);
		propComponent->SetStaticMesh(meshVariants[randomMeshVariantIndex]);
		propComponent->RegisterComponent();
		propComponent->AttachToComponent(OwnerVolume->GetBoxComponent(), FAttachmentTransformRules::KeepWorldTransform);
		
		FHitResult hit;
		const FVector relativeSpawnLocation(point.X - extent.X, point.Y - extent.Y, 0.0f);
		const FVector spawnLocation = relativeSpawnLocation.RotateAngleAxis(actorRotation.Yaw, FVector::UpVector) + actorLocation;
		const FVector extentZ(0.0f, 0.0f, extent.Z);
		GetWorld()->LineTraceSingleByChannel(hit, spawnLocation + extentZ, spawnLocation - extentZ, ECollisionChannel::ECC_GameTraceChannel1);

		propComponent->SetWorldLocation(hit.Location);
		
		if (bRandomRotation) 
		{
			propComponent->SetWorldRotation(FRotator(0.0f, randomStream.FRandRange(-180.0f, 180.0f), 0.0f));
		}

		OwnerVolume->AddInstanceComponent(propComponent);

		PropsMeshComponents.Add(propComponent);
	}
}

void UTerrainGeneratorComponent::ClearProps()
{
	for (auto& component : PropsMeshComponents) 
	{
		if (!component.IsNull())
			component->DestroyComponent();
	}

	PropsMeshComponents.Empty();
}

#if WITH_EDITOR
void UTerrainGeneratorComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (propertyName == GET_MEMBER_NAME_CHECKED(UTerrainGeneratorComponent, Seed) || propertyName == GET_MEMBER_NAME_CHECKED(UTerrainGeneratorComponent, GeneratorDataAsset))
	{
		GenerateProps();
	}
}
#endif