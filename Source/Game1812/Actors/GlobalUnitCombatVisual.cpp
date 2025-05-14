#include "GlobalUnitCombatVisual.h"

#include "../Pawns/Unit/Components/UnitCombatVisualComponent.h"
#include "../Pawns/Unit/Components/UnitVisualMeshComponent.h"

AGlobalUnitCombatVisual* AGlobalUnitCombatVisual::Instance = nullptr;

AGlobalUnitCombatVisual* AGlobalUnitCombatVisual::GetInstance(UWorld* World)
{
	if (Instance)
		return Instance;

	Instance = World->SpawnActor<AGlobalUnitCombatVisual>(AGlobalUnitCombatVisual::StaticClass());

	return Instance;
}

AGlobalUnitCombatVisual::AGlobalUnitCombatVisual()
{
	PrimaryActorTick.bCanEverTick = true;

	CrammingDistance = 6.0f;
	CrammingDistanceSquared = CrammingDistance * CrammingDistance;
}

void AGlobalUnitCombatVisual::BeginPlay()
{
	Super::BeginPlay();
	
	Instance = this;
}

void AGlobalUnitCombatVisual::BeginDestroy()
{
	Super::BeginDestroy();

	Instance = nullptr;
}

void AGlobalUnitCombatVisual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (TWeakObjectPtr<UUnitVisualBaseComponent>& component : VisualComponents)
	{
		if (!component.IsValid())
			continue;

		component->UpdateVisual(DeltaTime);
	}

	for (int a = 0; a < MeshComponents.Num(); a++) 
	{
		UUnitVisualMeshComponent* const component1 = MeshComponents[a].Get();

		//Remove all nullptr to unit's meshes, cleanup here, because it's faster to find nullptr, that ptr in given array
		if (!component1)
		{
			MeshComponents.RemoveAll([](TWeakObjectPtr<UUnitVisualMeshComponent>& el) { return el == nullptr; });
			a--;
			continue;
		}

		if (!component1->IsVisible())
			continue;

		for (int b = a + 1; b < MeshComponents.Num(); b++)
		{
			UUnitVisualMeshComponent* const component2 = MeshComponents[b].Get();

			if (!component2 || !component2->IsVisible())
				continue;

			FVector delta = component1->GetComponentLocation() - component2->GetComponentLocation();
			delta.Z = 0;

			const float deltaSizeSquared = delta.SizeSquared2D();

			if (deltaSizeSquared < CrammingDistanceSquared)
			{
				const float deltaSize = FMath::Sqrt(deltaSizeSquared);
				const FVector deltaNormalized = (deltaSize > 0.01f) ? (delta / deltaSize) : ((b & 1) ? FVector::ForwardVector : FVector::RightVector);

				const float pushDistance = (CrammingDistance - deltaSize) * 0.5;

				const FVector pushVector = deltaNormalized * pushDistance;

				component1->AddWorldOffset(pushVector);
				component2->AddWorldOffset(-pushVector);
			}
		}
	}

	for (TWeakObjectPtr<UUnitVisualMeshComponent>& component : MeshComponents)
	{
		if (!component.IsValid() || !component->IsVisible())
			continue;

		FHitResult hit;

		const FVector componentLocation = component->GetComponentLocation();
		
		GetWorld()->LineTraceSingleByChannel(hit, componentLocation + FVector(0.0f, 0.0f, 250.0f), componentLocation - FVector(0.0f, 0.0f, 250.0f), ECollisionChannel::ECC_GameTraceChannel1);

		if (hit.bBlockingHit)
			component->SetWorldLocation(hit.Location);
	}
}

void AGlobalUnitCombatVisual::AddVisualComponent(UUnitVisualBaseComponent* NewVisualComponent, UUnitVisualMeshComponent* NewMeshComponent)
{
	VisualComponents.Add(NewVisualComponent);
	MeshComponents.Add(NewMeshComponent);
}

void AGlobalUnitCombatVisual::AddVisualComponent(UUnitVisualBaseComponent* NewVisualComponent, const TArray<UUnitVisualMeshComponent*>& NewMeshComponents)
{
	VisualComponents.Add(NewVisualComponent);
	MeshComponents.Append(NewMeshComponents);
}

void AGlobalUnitCombatVisual::RemoveVisualComponent(UUnitVisualBaseComponent* VisualComponentToRemove)
{
	VisualComponents.Remove(VisualComponentToRemove);
}