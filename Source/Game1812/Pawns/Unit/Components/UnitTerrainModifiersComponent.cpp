#include "UnitTerrainModifiersComponent.h"

#include "../BaseUnit.h"
#include "../../../Actors/Tools/TerrainPropertyVolume.h"
#include "../../../DataAssets/TerrainPropertyDataAsset.h"

#include <Components/BoxComponent.h>

void FUnitTerrainModifiers::Reset(float Value)
{
	MovementSpeedModifier = Value;
	RotationSpeedModifier = Value;
	DamageModifier = Value;
}


UUnitTerrainModifiersComponent::UUnitTerrainModifiersComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UUnitTerrainModifiersComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerUnit = Cast<ABaseUnit>(GetOwner());

	if (!OwnerUnit)
		return;

	OwnerUnit->GetColliderComponent()->OnComponentBeginOverlap.AddDynamic(this, &UUnitTerrainModifiersComponent::OnBeginOverlap);
	OwnerUnit->GetColliderComponent()->OnComponentEndOverlap.AddDynamic(this, &UUnitTerrainModifiersComponent::OnEndOverlap);
}

void UUnitTerrainModifiersComponent::UpdateModifiers()
{
	if (OverlappedTerrainVolumes.IsEmpty())
		return Modifiers.Reset();
	
	Modifiers.Reset(50.f);

	for (ATerrainPropertyVolume* terrain : OverlappedTerrainVolumes) 
	{
		const UTerrainPropertyDataAsset* data = terrain->GetTerrainPropertyDataAsset();

		if (!data)
			continue;

		Modifiers.MovementSpeedModifier = FMath::Min(Modifiers.MovementSpeedModifier, data->GetMovementSpeedModifier(OwnerUnit->GetUnitType()));
		Modifiers.RotationSpeedModifier = FMath::Min(Modifiers.RotationSpeedModifier, data->GetRotationSpeedModifier(OwnerUnit->GetUnitType()));
		Modifiers.DamageModifier = FMath::Min(Modifiers.DamageModifier, data->GetDamageModifier(OwnerUnit->GetUnitType()));
	}
}

void UUnitTerrainModifiersComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OverlappedComponent || !OtherActor)
		return;

	ATerrainPropertyVolume* terrain = Cast<ATerrainPropertyVolume>(OtherActor);

	if (!terrain)
		return;

	OverlappedTerrainVolumes.Add(terrain);

	UpdateModifiers();
}

void UUnitTerrainModifiersComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OverlappedComponent || !OtherActor)
		return;

	ATerrainPropertyVolume* terrain = Cast<ATerrainPropertyVolume>(OtherActor);

	if (!terrain)
		return;

	OverlappedTerrainVolumes.Remove(terrain);

	UpdateModifiers();
}

