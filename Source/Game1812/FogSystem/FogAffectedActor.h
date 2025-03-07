#pragma once

#include "FogAffectedActor.generated.h"

UINTERFACE()
class GAME1812_API UFogAffectedActor : public UInterface
{
    GENERATED_BODY()
};

class GAME1812_API IFogAffectedActor
{
    GENERATED_BODY()

public:

    virtual void OnBeingCoveredInFog() = 0;
    virtual void OnBeingRevealedFromFog() = 0;
    virtual bool IsCoveredInFog() = 0;

    virtual TArray<class IFogAffectedComponent*>* GetFogAffectedComponents() { return nullptr; }
};

