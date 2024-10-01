#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitPathArrow.generated.h"

UCLASS()
class GAME1812_API AUnitPathArrow : public AActor
{
	GENERATED_BODY()
	
public:	

	AUnitPathArrow();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USplineComponent* SplinePathComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class USplineMeshComponent*> SplineMeshComponents;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* ArrowOutlineStartStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* ArrowOutlineMiddleStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* ArrowOutlineEndStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> PathPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> TempPathPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SegmentLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ArrowLocationHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector StartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector EndPoint;

	virtual void BeginPlay() override;

	void BuildArrow();

	void GetPathPoints(TArray<FVector>& PointsArray, const FVector& From, const FVector& To);

	void UpdateSplineWithPath();

	FVector ProjectPointToMap(const FVector& Point);

public:	

	virtual void Tick(float DeltaTime) override;

	void SetStartPoint(const FVector& NewStartPoint);
	void SetEndPoint(const FVector& NewEndPoint, bool ClearPoints = true, bool IsTemp = false);
};
