#include "PieceOrderWidgetComponent.h"

#include "../Piece.h"
#include "../../../UI/BaseOrderWidget.h"

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

	OwnerPiece->AddOnSelectedHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOrderWidgetComponent::OnEnable));

	OwnerPiece->AddOnSelectionRemovedHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOrderWidgetComponent::OnDisable));
	OwnerPiece->AddOnOrderAssignHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOrderWidgetComponent::OnDisable));
	OwnerPiece->AddOnUnitDeathHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOrderWidgetComponent::OnDisable));

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
	
}
