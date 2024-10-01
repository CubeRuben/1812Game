#include "CossacksGameInstance.h"

#include "Pawns/Unit/Units/AdjutantUnit.h"
#include "Actors/Pieces/ScoutPiece.h"
#include "Actors/Pieces/CombatPiece.h"
#include "Actors/UnitPathArrow.h"
#include "Actors/GhostPiece.h"
#include "Actors/EnemyPiece.h"


UClass* UCossacksGameInstance::GetCombatUnitPieceClass() const
{
	return CombatUnitPieceClass.Get();
}

UClass* UCossacksGameInstance::GetScoutUnitPieceClass() const
{
	return ScoutUnitPieceClass.Get();
}

UClass* UCossacksGameInstance::GetAdjutantUnitClass() const
{
	return AdjutantUnitClass.Get();
}

UClass* UCossacksGameInstance::GetUnitPathArrowClass() const
{
	return UnitPathArrowClass.Get();
}

UClass* UCossacksGameInstance::GetGhostPieceClass() const
{
	return GhostPieceClass.Get();
}

UClass* UCossacksGameInstance::GetEnemyPieceClass() const
{
	return EnemyPieceClass.Get();
}
