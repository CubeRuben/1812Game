#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../TeamEnum.h"
#include "../UnitTypeEnum.h"
#include "Damageable.generated.h"


UINTERFACE()
class GAME1812_API UDamageable : public UInterface
{
    GENERATED_BODY()
};

class GAME1812_API IDamageable
{
    GENERATED_BODY()

public:

    virtual float ApplyDamage(IDamageable* Attacker, float DamageAmount) = 0;

    virtual ETeam GetTeam() const = 0;
    virtual ECombatUnitType GetUnitType() const = 0;
    virtual FVector GetLocation() = 0;
    virtual bool IsValidTarget() = 0;
    virtual float GetDefense() = 0;

    virtual bool IsEnemy(ETeam Team) { return GetTeam() != Team; }
    
};