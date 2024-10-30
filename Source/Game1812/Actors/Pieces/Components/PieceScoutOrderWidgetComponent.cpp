#include "PieceScoutOrderWidgetComponent.h"

#include "../Piece.h"

UPieceScoutOrderWidgetComponent::UPieceScoutOrderWidgetComponent()
{
}

void UPieceScoutOrderWidgetComponent::AssignOrder(UUnitOrder* UnitOrder)
{
	if (!UnitOrder)
		return;

	OwnerPiece->AssignOrder(UnitOrder);
}
