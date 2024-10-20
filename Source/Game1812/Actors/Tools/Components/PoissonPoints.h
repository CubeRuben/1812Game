#pragma once

#include "CoreMinimal.h"
#include "PoissonPoints.generated.h"

USTRUCT()
struct FPoissonPoints
{
    GENERATED_BODY()

public:

	FPoissonPoints();
	FPoissonPoints(FRandomStream& NewRandomStream, float NewRadius, float Width, float Height);

protected:

	TArray<FVector2D> Points;

	float Radius;
	float CellSize;
	float Width;
	float Height;

	FRandomStream* RandomStream;

	FVector2D GenerateRandomPointAround(const FVector2D& Point);

	bool IsValidPoint(const FVector2D& Point, const TArray<FVector2D>& Grid, int32 GridWidth, int32 GridHeight);

	FIntPoint GetCellCoords(const FVector2D& Point);

public:

	TArray<FVector2D>& GetPoints() { return Points; };

	void GeneratePoissonPoints(int32 MaxAttempts = 20);

};

