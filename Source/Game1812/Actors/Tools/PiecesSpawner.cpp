#include "PiecesSpawner.h"

#include "../../CossacksGameInstance.h"
#include "../Pieces/ScoutPiece.h"
#include "../Pieces/CombatPiece.h"

APiecesSpawner::APiecesSpawner() :
	PieceToSpawn(EPieceToSpawn::COMBAT),
	CombatUnitData(nullptr),
	Amount(0),
	SpawnOffset(0.0f)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root Component"));
}

void APiecesSpawner::SpawnPieces()
{
	const int SideLength = FMath::CeilToInt(FMath::Sqrt((float)Amount));

	const FVector centerOffset = FVector(1.0f, 1.0f, 0.0f) * SpawnOffset * 0.5f;
	const FVector gridOffset = FVector(1.0f, 1.0f, 0.0f) * (SideLength * 0.5f) * SpawnOffset;
	const FVector rootLocation = GetActorLocation();
	const FVector totalOffset = centerOffset - gridOffset + rootLocation;

	for (int x = 0, i = 0; x < SideLength; x++)
	{
		for (int y = 0; (y < SideLength) && (i < Amount); y++, i++)
		{
			const FVector spawnLocation = FVector(x, y, 0.0f) * SpawnOffset;
			SpawnPiece(spawnLocation + totalOffset);
		}
	}
}

void APiecesSpawner::SpawnPiece(const FVector& Location)
{
	UCossacksGameInstance* const gameInstance = GetGameInstance<UCossacksGameInstance>();

	if (!gameInstance)
		return;

	const FRotator randomRotation(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f);

	if (PieceToSpawn == EPieceToSpawn::COMBAT)
	{
		ACombatPiece* const combatPiece = GetWorld()->SpawnActor<ACombatPiece>(gameInstance->GetCombatUnitPieceClass(), Location, randomRotation);

		if (!combatPiece)
			return;

		combatPiece->SetCombatUnitData(CombatUnitData);

		return;
	}
	
	if (PieceToSpawn == EPieceToSpawn::SCOUT) 
	{
		GetWorld()->SpawnActor<AScoutPiece>(gameInstance->GetScoutUnitPieceClass(), Location, FRotator::ZeroRotator);

		return;
	}
}
