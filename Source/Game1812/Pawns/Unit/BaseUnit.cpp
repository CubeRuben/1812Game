#include "BaseUnit.h"

#include "Orders/UnitOrder.h"
#include "Components/UnitMovementComponent.h"
#include "Components/UnitTerrainModifiersComponent.h"
#include "../../Actors/Pieces/Piece.h"
#include "../../Actors/HeadQuarters.h"
#include "../../Actors/UnitDeathNotifier.h"

#include <Components/BoxComponent.h>

ABaseUnit::ABaseUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxComponent->InitBoxExtent(FVector(10, 10, 5));
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionObjectType(ECC_Pawn);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = BoxComponent;

	MovementComponent = CreateDefaultSubobject<UUnitMovementComponent>(TEXT("Movement Component"));

	TerrainModifiersComponent = CreateDefaultSubobject<UUnitTerrainModifiersComponent>(TEXT("Terrain Modifiers Component"));

	Team = ETeam::Russia;
}

void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();

	AddActorWorldOffset(FVector(0, 0, 10));
	AddActorWorldOffset(FVector(0, 0, -20), true);
}

void ABaseUnit::OnUnitDeath()
{
	AUnitDeathNotifier* notifier = GetWorld()->SpawnActor<AUnitDeathNotifier>(AUnitDeathNotifier::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
	notifier->SetPiece(OwnerPiece.Get());
}

void ABaseUnit::OnBeingCoveredInFog()
{
	SetActorHiddenInGame(true);

	OnUnitFogChange.Broadcast(true);
}

void ABaseUnit::OnBeingRevealedFromFog()
{
	SetActorHiddenInGame(false);

	OnUnitFogChange.Broadcast(false);
}

bool ABaseUnit::IsCoveredInFog()
{
	return IsHidden();
}

const FUnitTerrainModifiers& ABaseUnit::GetTerrainModifiers() const
{
	return TerrainModifiersComponent->GetTerrainModifiers();
}

void ABaseUnit::SetOwnerPiece(APiece* NewOwnerPiece) { OwnerPiece = NewOwnerPiece; }
