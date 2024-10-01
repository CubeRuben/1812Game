#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMultipleSelectionDelegate);

UCLASS()
class GAME1812_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerInteractionComponent();

protected:

	class APlayerPawn* PlayerPawn;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionDistance;

	UPROPERTY(EditAnywhere, Category = "Dragging")
	float DraggingHeight;

	UPROPERTY(EditAnywhere, Category = "Dragging")
	float AltDraggingHeight;

	UPROPERTY(EditAnywhere, Category = "Dragging")
	float RotateSpeed;

	UPROPERTY(EditAnywhere, Category = "Selection")
	bool bIsMultipleSelection;

	UPROPERTY(VisibleAnywhere, Category = "Selection")
	TArray<AActor*> InteractableActorsSelectedGroup;

	class IInteractable* CurrentDraggable;
	class IInteractable* CurrentHovered;
	class IInteractable* CurrentSelected;

	UPROPERTY(BlueprintAssignable)
	FMultipleSelectionDelegate OnMultipleSelectionStart;
	
	UPROPERTY(BlueprintAssignable)
	FMultipleSelectionDelegate OnMultipleSelectionEnd;

	void SetCurrentDraggable(class IInteractable* NewDraggable);
	void SetCurrentHovered(class IInteractable* NewHovered);
	void SetCurrentSelected(class IInteractable* NewSelected);

	virtual void BeginPlay() override;

	FHitResult SingleCursorTrace();
	void FindInteractableAtCursor(AActor*& Actor, class IInteractable*& Interactable);

	FVector GetSurfaceUnderActor(AActor* Actor);

public:	

	void ClearSelectedGroup();

	UFUNCTION(BlueprintCallable)
	void SetSelectedGroup(UPARAM(Ref) const TArray<TScriptInterface<class IInteractable>>& NewGroup);

	const TArray<AActor*>& GetSelectedGroup() const { return InteractableActorsSelectedGroup; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
