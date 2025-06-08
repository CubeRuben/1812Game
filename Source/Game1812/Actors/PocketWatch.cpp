#include "PocketWatch.h"

#include "../CossacksGameMode.h"

#include <Kismet/GameplayStatics.h>

APocketWatch::APocketWatch()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	WatchMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Watch Mesh"));
	WatchMeshComponent->SetupAttachment(RootComponent);

	ArrowSecMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Sec Mesh"));
	ArrowSecMeshComponent->SetupAttachment(RootComponent);

	ArrowMinMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Min Mesh"));
	ArrowMinMeshComponent->SetupAttachment(RootComponent);

	ArrowHourMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Hour Mesh"));
	ArrowHourMeshComponent->SetupAttachment(RootComponent);
}

void APocketWatch::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ACossacksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void APocketWatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GameMode)
		return;

	ArrowSecMeshComponent->SetRelativeRotation(FRotator(0.0f, GameMode->GetGameTimeMinutes() * 360.0f, 0.0f));
	ArrowMinMeshComponent->SetRelativeRotation(FRotator(0.0f, GameMode->GetGameTimeMinutes() * 6.0f, 0.0f));
	ArrowHourMeshComponent->SetRelativeRotation(FRotator(0.0f, GameMode->GetGameTimeMinutes() * 0.5f, 0.0f));
}

