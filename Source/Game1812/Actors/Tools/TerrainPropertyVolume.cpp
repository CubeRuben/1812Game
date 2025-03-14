#include "TerrainPropertyVolume.h"

#include "../../DataAssets/TerrainPropertyDataAsset.h"
#include "NavArea_CustomTerrain.h"
#include <Components/BoxComponent.h>
#include <NavModifierComponent.h>

ATerrainPropertyVolume::ATerrainPropertyVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Terrain Volume"));
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);
	
	NavModifierComponent = CreateDefaultSubobject<UNavModifierComponent>(TEXT("Navigation Mesh Modifier"));
	NavModifierComponent->AreaClass = UNavArea_CustomTerrain::StaticClass();
}

void ATerrainPropertyVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!TerrainPropertyDataAsset)
		return;

	NavModifierComponent->AreaClass = TerrainPropertyDataAsset->GetTerrainNavAreaClass();
}