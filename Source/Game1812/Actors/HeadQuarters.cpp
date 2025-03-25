#include "HeadQuarters.h"

#include "../OrdersSenderComponent.h"
#include "../FogSystem/FogOfWar.h"

AHeadQuarters::AHeadQuarters()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	OrdersSenderComponent = CreateDefaultSubobject<UOrdersSenderComponent>(TEXT("Orders Sender Component"));
}

void AHeadQuarters::BeginPlay()
{
	Super::BeginPlay();	
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(OrdersSenderComponent, &UOrdersSenderComponent::UpdateFogOfWar));
}