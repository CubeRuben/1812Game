#pragma once

#include "FogAffectedComponent.generated.h"

UINTERFACE()
class GAME1812_API UFogAffectedComponent : public UInterface
{
    GENERATED_BODY()
};

class GAME1812_API IFogAffectedComponent
{
    GENERATED_BODY()

public:

    virtual TArray<USceneComponent*>* GetSceneComponents() = 0;
    virtual void OnComponentBeingCoveredInFog(USceneComponent* Component) = 0;
    virtual void OnComponentBeingRevealedFromFog(USceneComponent* Component) = 0;
};

