#include "TutorialScoutDiscoverPoint.h"

ATutorialScoutDiscoverPoint::ATutorialScoutDiscoverPoint() :
	PointActive(false)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetRenderCustomDepth(true);
	StaticMeshComponent->SetCustomDepthStencilValue(12);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATutorialScoutDiscoverPoint::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMeshComponent->SetVisibility(PointActive);
}

void ATutorialScoutDiscoverPoint::SetPointActive(bool NewActive)
{
	PointActive = NewActive;
	StaticMeshComponent->SetVisibility(PointActive);
}

void ATutorialScoutDiscoverPoint::OnBeingRevealedFromFog()
{
	OnPointDiscover.Broadcast();

	Destroy();
}
