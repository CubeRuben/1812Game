#include "PieceOrderWidgetComponent.h"

#include "../Piece.h"
#include "../../../UI/BaseOrderWidget.h"
#include "../../../Pawns/Unit/Orders/UnitOrder.h"
#include "../../../Pawns/Player/PlayerPawn.h"
#include "../../../Pawns/Player/Components/PlayerInteractionComponent.h"

UPieceOrderWidgetComponent::UPieceOrderWidgetComponent()
{
	SetWidgetSpace(EWidgetSpace::Screen);
	SetDrawSize(FVector2D(200.f, 200.f));

	OnDisable();
}

void UPieceOrderWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPiece = Cast<APiece>(GetOwner());

	if (!OwnerPiece)
		return DestroyComponent();

	OwnerPiece->AddOnSelectedHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOrderWidgetComponent::OnEnable));

	OwnerPiece->AddOnSelectionRemovedHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOrderWidgetComponent::OnDisable));
	OwnerPiece->AddOnOrderAssignHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOrderWidgetComponent::OnDisable));
	OwnerPiece->AddOnUnitDeathHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOrderWidgetComponent::OnDisable));

	UBaseOrderWidget* orderWidget = Cast<UBaseOrderWidget>(GetWidget());

	if (orderWidget)
		orderWidget->Init(this);
}

void UPieceOrderWidgetComponent::OnEnable()
{
	if (OwnerPiece->IsPlayed())
		SetVisibility(true);
}

void UPieceOrderWidgetComponent::OnDisable()
{
	SetVisibility(false);
}

void UPieceOrderWidgetComponent::AssignOrder(UUnitOrder* UnitOrder)
{
	if (!UnitOrder)
		return;

	OwnerPiece->AssignOrder(UnitOrder);

	APlayerPawn* playerPawn = APlayerPawn::GetInstance();

	if (!playerPawn)
		return;

	const TArray<AActor*> group = playerPawn->GetInteractionComponent()->GetSelectedGroup();

	if (!group.Contains(OwnerPiece))
		return;

	for (AActor* actor : group) 
	{
		if (actor == OwnerPiece)
			continue;

		APiece* piece = Cast<APiece>(actor);

		if (piece)
			piece->AssignOrder(DuplicateObject(UnitOrder, this));
	}
}
