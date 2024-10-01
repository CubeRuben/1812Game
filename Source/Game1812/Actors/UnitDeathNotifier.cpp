#include "UnitDeathNotifier.h"

#include "Pieces/Piece.h"

AUnitDeathNotifier::AUnitDeathNotifier()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
}

void AUnitDeathNotifier::SetPiece(APiece* NewPiece)
{
	PieceToNotify = NewPiece;
}

void AUnitDeathNotifier::OnBeingCoveredInFog()
{
	Destroy();
}

void AUnitDeathNotifier::OnBeingRevealedFromFog()
{
	if (PieceToNotify.IsValid())
		//HEAVY IS DEAD
		PieceToNotify->OnDeathUnit();

	Destroy();
}

bool AUnitDeathNotifier::IsCoveredInFog()
{
	return true;
}

