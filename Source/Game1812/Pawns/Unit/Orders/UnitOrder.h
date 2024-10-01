#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssignedUnitOrder.h"
#include "UnitOrder.generated.h"

UENUM(BlueprintType)
enum class EUnitEnemyReaction : uint8 {
	Attack = 0	UMETA(DisplayName = "Attack"),
	Defend = 1	UMETA(DisplayName = "Defend"),
};

UCLASS(BlueprintType, Abstract)
class GAME1812_API UUnitOrder : public UObject
{
	GENERATED_BODY()

public:

	UUnitOrder();
};

UCLASS(BlueprintType)
class GAME1812_API UCombatUnitOrder : public UUnitOrder
{
	GENERATED_BODY()

public:

	UCombatUnitOrder();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUnitEnemyReaction UnitEnemyReaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bForcedMarch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bConcentratedFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YawRotation;

};

UCLASS(BlueprintType, Blueprintable)
class GAME1812_API UScoutUnitOrder : public UUnitOrder
{
	GENERATED_BODY()

public:

	UScoutUnitOrder();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> ExplorationLocations;
};

UCLASS(BlueprintType)
class GAME1812_API UAdjutantUnitOrder : public UUnitOrder
{
	GENERATED_BODY()

public:

	UAdjutantUnitOrder();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAssignedCombatUnitOrder> SentOrdersToUnits;
};