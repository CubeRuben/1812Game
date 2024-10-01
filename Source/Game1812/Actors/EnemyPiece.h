#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyPiece.generated.h"

UCLASS()
class GAME1812_API AEnemyPiece : public AActor
{
	GENERATED_BODY()
	
public:	

	AEnemyPiece();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* FoundationMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* FigureMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Height;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsCoveredInFog;

	TWeakObjectPtr<class ACombatUnit> Unit;

	FVector GetTargetLocation();

	UFUNCTION()
	void OnUnitDestroy(AActor* Actor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCoveredInFogChange(bool IsCovered);

public:	

	void SetCombatUnit(ACombatUnit* NewUnit);

	virtual void Tick(float DeltaTime) override;

};
