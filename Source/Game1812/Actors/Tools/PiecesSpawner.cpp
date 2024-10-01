#include "PiecesSpawner.h"

#include "../../CossacksGameInstance.h"
#include "../Pieces/ScoutPiece.h"
#include "../Pieces/CombatPiece.h"

APiecesSpawner::APiecesSpawner()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root Component"));

	PieceToSpawn = EPieceToSpawn::COMBAT;
	CombatUnitData = nullptr;
	SpawnInterval = 40.f;
}

void APiecesSpawner::BeginPlay()
{
	Super::BeginPlay();

	const int SideLength = FMath::CeilToInt(FMath::Sqrt((float)Amount));

	for (int x = 0, i = 0; x < SideLength; x++) 
	{
		for (int y = 0; (y < SideLength) && (i < Amount); y++, i++)
		{
			const FVector SpawnLocation = FVector(x, y, 0) * SpawnInterval;
			const FVector CenterOffset = FVector(1, 1, 0) * SpawnInterval / 2.f;
			const FVector GridOffset = FVector(1, 1, 0) * (SideLength / 2.f) * SpawnInterval;

			SpawnPiece(SpawnLocation + CenterOffset - GridOffset + GetActorLocation());
		}
	}
}

void APiecesSpawner::SpawnPiece(const FVector& Location)
{
	UCossacksGameInstance* gameInstance = GetGameInstance<UCossacksGameInstance>();

	if (!gameInstance)
		return;

	switch (PieceToSpawn)
	{
	case EPieceToSpawn::SCOUT:
		GetWorld()->SpawnActor<AScoutPiece>(gameInstance->GetScoutUnitPieceClass(), Location, FRotator::ZeroRotator);
		break;

	case EPieceToSpawn::COMBAT:
		ACombatPiece* piece = GetWorld()->SpawnActor<ACombatPiece>(gameInstance->GetCombatUnitPieceClass(), Location, FRotator::ZeroRotator);

		if (piece)
			piece->SetCombatUnitData(CombatUnitData);

		break;
	}
}
