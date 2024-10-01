#pragma once

#include "Interactable.generated.h"

UINTERFACE(NotBlueprintable, BlueprintType)
class GAME1812_API UInteractable : public UInterface
{
    GENERATED_BODY()
};

class GAME1812_API IInteractable
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    virtual void StartDragging() = 0;

    UFUNCTION(BlueprintCallable)
    virtual void StopDragging() = 0;


	UFUNCTION(BlueprintCallable)
    virtual void StartCursorHover() = 0;

    UFUNCTION(BlueprintCallable)
    virtual void StopCursorHover() = 0;


    UFUNCTION(BlueprintCallable)
    virtual void StartGroupSelectionHover() = 0;

    UFUNCTION(BlueprintCallable)
    virtual void StopGroupSelectionHover() = 0;


    UFUNCTION(BlueprintCallable)
    virtual void Selected() = 0;

    UFUNCTION(BlueprintCallable)
    virtual void SelectionRemoved() = 0;

    virtual FVector GetDragOffset() = 0;

    UFUNCTION(BlueprintCallable)
    virtual bool CanBeGrouped() = 0;
};
