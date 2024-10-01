#include "PaperMap.h"

#include <Components/BoxComponent.h>

APaperMap::APaperMap()
{
	PrimaryActorTick.bCanEverTick = false;

	PaperMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paper Mesh"));
	PaperMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	RootComponent = PaperMeshComponent;

	BordersComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Borders Box"));
	BordersComponent->SetupAttachment(RootComponent);
	BordersComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
	BordersComponent->ComponentTags.Add("MapBorder");
}

void APaperMap::BeginPlay()
{
	Super::BeginPlay();
	
	
}
