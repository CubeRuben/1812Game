#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FormationMovement.generated.h"

UCLASS(BlueprintType)
class GAME1812_API UFormationMovement : public UObject
{
	GENERATED_BODY()

public:

	UFormationMovement();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class ACombatUnit>> CombatUnits;

	float FindMinimalMovementSpeed();
	float FindMinimalRotationSpeed();
	
public:

	float GetMovementSpeed();
	float GetRotationSpeed();
	
};
