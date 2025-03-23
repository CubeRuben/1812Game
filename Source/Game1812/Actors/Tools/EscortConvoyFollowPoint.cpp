#include "EscortConvoyFollowPoint.h"

AEscortConvoyFollowPoint::AEscortConvoyFollowPoint()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
}

void AEscortConvoyFollowPoint::BeginPlay()
{
	Super::BeginPlay();
	
}