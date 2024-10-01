#include "PieceOutlineComponent.h"

#include "../Piece.h"

UPieceOutlineComponent::UPieceOutlineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bIsCursorHovered = false;
	bIsGroupSelectionHovered = false;
	bIsBeingDragged = false;
	bIsSelected = false;
}

void UPieceOutlineComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerPiece = Cast<APiece>(GetOwner());

	if (!OwnerPiece)
		return DestroyComponent();

	OwnerPiece->AddOnStartCursorHoverHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnCursorHoverStart));
	OwnerPiece->AddOnStartGroupSelectionHoverHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnGroupSelectionHoverStart));
	OwnerPiece->AddOnStartDraggingHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnDragStart));
	OwnerPiece->AddOnSelectedHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnSelected));

	OwnerPiece->AddOnStopCursorHoverHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnCursorHoverStop));
	OwnerPiece->AddOnStopGroupSelectionHoverHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnGroupSelectionHoverStop));
	OwnerPiece->AddOnStopDraggingHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnDragStop));
	OwnerPiece->AddOnSelectionRemovedHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnSelectionRemoved));
}

void UPieceOutlineComponent::OnCursorHoverStart()
{
	bIsCursorHovered = true;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnCursorHoverStop()
{
	bIsCursorHovered = false;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnGroupSelectionHoverStart()
{
	bIsGroupSelectionHovered = true;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnGroupSelectionHoverStop()
{
	bIsGroupSelectionHovered = false;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnDragStart()
{
	bIsBeingDragged = true;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnDragStop()
{
	bIsBeingDragged = false;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnSelected()
{
	bIsSelected = true;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnSelectionRemoved()
{
	bIsSelected = false;

	UpdateOutlineState();
}

void UPieceOutlineComponent::UpdateOutlineState()
{
	SetOutlineEnabled(bIsCursorHovered || bIsGroupSelectionHovered || bIsBeingDragged || bIsSelected);
}

void UPieceOutlineComponent::SetOutlineEnabled(bool bIsEnabled)
{
	UStaticMeshComponent* meshComponent1 = OwnerPiece->GetPieceFigureMeshComponent();
	UStaticMeshComponent* meshComponent2 = OwnerPiece->GetPieceFoundationMeshComponent();

	if (bIsEnabled) 
	{
		const int depthStencilValue = GetDepthStencilValue();

		meshComponent1->SetRenderCustomDepth(true);
		meshComponent1->SetCustomDepthStencilValue(depthStencilValue);

		meshComponent2->SetRenderCustomDepth(true);
		meshComponent2->SetCustomDepthStencilValue(depthStencilValue);
		return;
	}

	meshComponent1->SetRenderCustomDepth(false);
	meshComponent1->SetCustomDepthStencilValue(0);

	meshComponent2->SetRenderCustomDepth(false);
	meshComponent2->SetCustomDepthStencilValue(0);
}

int UPieceOutlineComponent::GetDepthStencilValue()
{
	if (bIsGroupSelectionHovered)
		return 11;

	return 10;
}
