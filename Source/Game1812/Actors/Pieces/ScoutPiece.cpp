#include "ScoutPiece.h"

#include "Components/PiecePredictedPathComponent.h"
#include "Components/PieceScoutOrderWidgetComponent.h"

#include <Components/BoxComponent.h>

AScoutPiece::AScoutPiece()
{
	ScoutOrderWidgetComponent = CreateDefaultSubobject<UPieceScoutOrderWidgetComponent>(TEXT("Order Widget"));
	ScoutOrderWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	ScoutOrderWidgetComponent->SetupAttachment(BoxCollisionComponent);

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
