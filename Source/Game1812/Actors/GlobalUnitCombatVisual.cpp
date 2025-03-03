#include "GlobalUnitCombatVisual.h"

#include "../Pawns/Unit/Components/UnitCombatVisualComponent.h"

AGlobalUnitCombatVisual* AGlobalUnitCombatVisual::Instance = nullptr;

void AGlobalUnitCombatVisual::CreateInstance(UWorld* World)
{
	Instance = World->SpawnActor<AGlobalUnitCombatVisual>(AGlobalUnitCombatVisual::StaticClass());
}

void AGlobalUnitCombatVisual::AddVisualComponents(UUnitCombatVisualComponent* NewVisualComponent, const TArray<class UStaticMeshComponent*>& NewMeshComponents)
{
	VisualComponents.Add(NewVisualComponent);
	MeshComponents.Append(NewMeshComponents);
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

	for (UUnitCombatVisualComponent* const component : VisualComponents) 
	{
		component->UpdateVisual(DeltaTime);
	}

	for (int a = 0; a < MeshComponents.Num(); a++) 
	{
		for (int b = a + 1; b < MeshComponents.Num(); b++)
		{
			UStaticMeshComponent* const component1 = MeshComponents[a];
			UStaticMeshComponent* const component2 = MeshComponents[b];

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

	for (UStaticMeshComponent* const component : MeshComponents)
	{
		FHitResult hit;

		const FVector componentLocation = component->GetComponentLocation();

		GetWorld()->LineTraceSingleByChannel(hit, componentLocation + FVector(0.0f, 0.0f, 250.f), componentLocation - FVector(0.0f, 0.0f, 250.f), ECollisionChannel::ECC_GameTraceChannel1);

		if (hit.bBlockingHit)
		{
			component->SetWorldLocation(hit.Location);
		}
		else 
		{
			GEngine->AddOnScreenDebugMessage(100, 5.0f, FColor::Yellow, "AGlobalUnitCombatVisual::Tick - Unit Visual out of map");
		}
	}
}

