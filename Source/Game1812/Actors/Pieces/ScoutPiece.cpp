#include "ScoutPiece.h"

#include "Components/PiecePredictedPathComponent.h"

AScoutPiece::AScoutPiece()
{
	PredictedPathComponent->SetScout(true);
}

void AScoutPiece::AssignOrder(UUnitOrder* UnitOrder)
{
	Super::AssignOrder(UnitOrder);

	if (!Unit.IsValid())
		return;

	Unit->AssignOrder(UnitOrder);
}

void AScoutPiece::OnOrderPointAdded()
{
	OnOrderPointAdd.Broadcast();
}
