#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMultipleSelectionDelegate);

UENUM()
enum class EUnitsFormation : uint8
{
	Lines
};

UCLASS()
class GAME1812_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerInteractionComponent();

protected:

	class APlayerPawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dragging")
	float DraggingHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dragging")
	float AltDraggingHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dragging")
	float RotateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selection")
	bool bIsMultipleSelection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Selection")
	TArray<AActor*> InteractableActorsSelectedGroup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Limits")
	bool AllowedToInteract;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Limits")
	bool AllowedToDrag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Limits")
	bool AllowedToSelect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Limits", meta = (MustImplement = "Interactable"))
	TArray<TSubclassOf<UObject>> InteractableClassesWhitelist;

	class IInteractable* CurrentDraggable;
	class IInteractable* CurrentHovered;
	class IInteractable* CurrentSelected;

	UPROPERTY(BlueprintAssignable)
	FMultipleSelectionDelegate OnMultipleSelectionStart;
	
	UPROPERTY(BlueprintAssignable)
	FMultipleSelectionDelegate OnMultipleSelectionEnd;

	virtual void BeginPlay() override;

	FHitResult SingleCursorTrace(bool IgnoreGroup = false);
	void FindInteractableAtCursor(AActor*& Actor, class IInteractable*& Interactable);

	void HandleFormationInput();
	bool GetFormationOffsetArray(int FormationType, int Amount, TArray<FVector>& OutArray);

	FVector GetSurfaceUnderActor(AActor* Actor);

public:	

	void SetCurrentDraggable(class IInteractable* NewDraggable);
	void SetCurrentHovered(class IInteractable* NewHovered);
	void SetCurrentSelected(class IInteractable* NewSelected);

	bool IsAllowedToInteract() const { return AllowedToInteract; }
	void SetAllowedToInteract(bool NewAllowedToInteract);

	bool IsAllowedToDrag() const { return AllowedToDrag; }
	void SetAllowedToDrag(bool NewAllowedToDrag);

	bool IsAllowedToSelect() const { return AllowedToSelect; }
	void SetAllowedToSelect(bool NewAllowedToSelect);

	void AddInteractableClassToWhitelist(TSubclassOf<UObject> Class);
	void RemoveInteractableClassFromWhitelist(TSubclassOf<UObject> Class);
	bool IsInteractableClassAllowed(class IInteractable* Interactable);
	bool IsInteractableClassAllowed(UObject* Object);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsInteractableClassAllowed(TScriptInterface<class IInteractable> Interactable);

	void ClearSelectedGroup();

	UFUNCTION(BlueprintCallable)
	void SetSelectedGroup(UPARAM(Ref) const TArray<TScriptInterface<class IInteractable>>& NewGroup);

	const TArray<AActor*>& GetSelectedGroup() const { return InteractableActorsSelectedGroup; }

	void RemoveInteractable(IInteractable* Interactable);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
