#include "PieceOutlineComponent.h"

#include "../Piece.h"

UPieceOutlineComponent::UPieceOutlineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bCursorHovered = false;
	bGroupSelectionHovered = false;
	bBeingDragged = false;
	bSelected = false;
}

void UPieceOutlineComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerPiece = Cast<APiece>(GetOwner());

	if (!OwnerPiece)
		return DestroyComponent();

	OwnerPiece->AddOnStartCursorHoverHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnCursorHoverStart));
	OwnerPiece->AddOnStartGroupSelectionHoverHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnGroupSelectionHoverStart));
	OwnerPiece->AddOnStartDraggingHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnDragStart));
	OwnerPiece->AddOnSelectedHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnSelected));

	OwnerPiece->AddOnStopCursorHoverHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnCursorHoverStop));
	OwnerPiece->AddOnStopGroupSelectionHoverHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnGroupSelectionHoverStop));
	OwnerPiece->AddOnStopDraggingHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnDragStop));
	OwnerPiece->AddOnSelectionRemovedHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPieceOutlineComponent::OnSelectionRemoved));
}

void UPieceOutlineComponent::OnCursorHoverStart()
{
	bCursorHovered = true;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnCursorHoverStop()
{
	bCursorHovered = false;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnGroupSelectionHoverStart()
{
	bGroupSelectionHovered = true;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnGroupSelectionHoverStop()
{
	bGroupSelectionHovered = false;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnDragStart()
{
	bBeingDragged = true;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnDragStop()
{
	bBeingDragged = false;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnSelected()
{
	bSelected = true;

	UpdateOutlineState();
}

void UPieceOutlineComponent::OnSelectionRemoved()
{
	bSelected = false;

	UpdateOutlineState();
}

void UPieceOutlineComponent::UpdateOutlineState()
{
	SetOutlineEnabled(bCursorHovered || bGroupSelectionHovered || bBeingDragged || bSelected || bAttentionEnabled);
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
	if (bGroupSelectionHovered)
		return 11;

	if (bCursorHovered || bBeingDragged || bSelected)
		return 10;

	if (bAttentionEnabled)
		return 12;

	return 0;
}

void UPieceOutlineComponent::SetAttentionEnabled(bool NewAttentionEnabled)
{
	bAttentionEnabled = NewAttentionEnabled;

	UpdateOutlineState();
}
