#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyMoveTarget.generated.h"

UCLASS()
class GAME1812_API AEnemyMoveTarget : public AActor
{
	GENERATED_BODY()
	
public:	

	AEnemyMoveTarget();

protected:

	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<class ACombatUnit>> UnitsToMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeToMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveAreaBound;

public:	

	virtual void Tick(float DeltaTime) override;

};
