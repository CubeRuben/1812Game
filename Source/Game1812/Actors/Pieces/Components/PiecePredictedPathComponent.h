#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PiecePredictedPathComponent.generated.h"

UCLASS()
class GAME1812_API UPiecePredictedPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPiecePredictedPathComponent();

protected:

	class APiece* OwnerPiece;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class AUnitPathArrow> UnitPathArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class AGhostPiece*> GhostPieces;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector PathStartPoint;

	bool bIsScout;

	virtual void BeginPlay() override;

	void SpawnArrow();
	void SpawnGhostPiece(const FVector& Location);

	void BuildArrow();
	void ScoutBuildArrow();
	
	void DestroyArrow();
	void ApplyArrow();

	void InitStartPoint();
public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetScout(bool NewScout);
};
