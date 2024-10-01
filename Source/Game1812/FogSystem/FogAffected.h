#pragma once

#include "FogAffected.generated.h"

UINTERFACE()
class GAME1812_API UFogAffected : public UInterface
{
    GENERATED_BODY()
};

class GAME1812_API IFogAffected
{
    GENERATED_BODY()

public:

    virtual void OnBeingCoveredInFog() = 0;
    virtual void OnBeingRevealedFromFog() = 0;
    virtual bool IsCoveredInFog() = 0;
};

