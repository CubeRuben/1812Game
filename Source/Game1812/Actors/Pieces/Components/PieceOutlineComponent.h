#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PieceOutlineComponent.generated.h"

UCLASS()
class GAME1812_API UPieceOutlineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPieceOutlineComponent();

protected:

	class APiece* OwnerPiece;

	bool bCursorHovered;
	bool bGroupSelectionHovered;
	bool bBeingDragged;
	bool bSelected;

	bool bAttentionEnabled;

	virtual void BeginPlay() override;

	void OnCursorHoverStart();
	void OnCursorHoverStop();

	void OnGroupSelectionHoverStart();
	void OnGroupSelectionHoverStop();

	void OnDragStart();
	void OnDragStop();

	void OnSelected();
	void OnSelectionRemoved();

	void UpdateOutlineState();

	void SetOutlineEnabled(bool bIsEnabled);

	int GetDepthStencilValue();

public:	
		
	bool IsAttentionEnabled() const { return bAttentionEnabled; }
	void SetAttentionEnabled(bool NewAttentionEnabled);

};
