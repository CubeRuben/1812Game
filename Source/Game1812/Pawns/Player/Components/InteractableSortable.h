#pragma once

#include "Interactable.h"
#include "InteractableSortable.generated.h"

UINTERFACE(NotBlueprintable, BlueprintType)
class GAME1812_API UInteractableSortable : public UInterface
{
    GENERATED_BODY()
};

class GAME1812_API IInteractableSortable
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    virtual int GetPriority() const = 0;

};
