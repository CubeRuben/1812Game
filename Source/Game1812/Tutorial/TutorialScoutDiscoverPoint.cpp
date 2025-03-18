#include "TutorialScoutDiscoverPoint.h"

ATutorialScoutDiscoverPoint::ATutorialScoutDiscoverPoint()
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
	
	StaticMeshComponent->SetVisibility(false);
}

bool ATutorialScoutDiscoverPoint::GetPointActive() const
{
	return StaticMeshComponent->IsVisible();
}

void ATutorialScoutDiscoverPoint::SetPointActive(bool NewActive)
{
	StaticMeshComponent->SetVisibility(NewActive);
}

void ATutorialScoutDiscoverPoint::OnBeingRevealedFromFog()
{
	OnPointDiscover.Broadcast();

	Destroy();
}
