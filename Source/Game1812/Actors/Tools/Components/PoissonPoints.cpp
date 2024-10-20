#include "PoissonPoints.h"

FPoissonPoints::FPoissonPoints()
{
}

FPoissonPoints::FPoissonPoints(FRandomStream& NewRandomStream, float NewRadius, float NewWidth, float NewHeight)
{
    RandomStream = &NewRandomStream;

    Radius = NewRadius;
    CellSize = Radius / FMath::Sqrt(2.0f);
    Width = NewWidth;
    Height = NewHeight;
}

void FPoissonPoints::GeneratePoissonPoints(int32 MaxAttempts)
{
    if (!RandomStream)
        return;
    
    TArray<FVector2D> activePoints;

    int32 gridWidth = FMath::CeilToInt(Width / CellSize);
    int32 gridHeight = FMath::CeilToInt(Height / CellSize);

    TArray<FVector2D> grid;
    grid.SetNumZeroed(gridWidth * gridHeight);

    FVector2D initialPoint(RandomStream->FRandRange(0.0f, Width), RandomStream->FRandRange(0.0f, Height));

    activePoints.Add(initialPoint);

    Points.Add(initialPoint);

    grid[GetCellCoords(initialPoint).Y * gridWidth + GetCellCoords(initialPoint).X] = initialPoint;

    while (activePoints.Num() > 0)
    {
        const int32 index = FMath::RandRange(0, activePoints.Num() - 1);
        FVector2D point = activePoints[index];
        bool bFound = false;

        for (int32 i = 0; i < MaxAttempts; i++)
        {
            FVector2D NewPoint = GenerateRandomPointAround(point);
            if (IsValidPoint(NewPoint, grid, gridWidth, gridHeight))
            {
                activePoints.Add(NewPoint);
                Points.Add(NewPoint);
                grid[GetCellCoords(NewPoint).Y * gridWidth + GetCellCoords(NewPoint).X] = NewPoint;
                bFound = true;
                break;
            }
        }

        if (!bFound)
        {
            activePoints.RemoveAt(index);
        }
    }
}


FVector2D FPoissonPoints::GenerateRandomPointAround(const FVector2D& Point)
{
    float Angle = RandomStream->FRandRange(0.0f, 2 * PI);
    float Dist = RandomStream->FRandRange(Radius, 2 * Radius);
    return FVector2D(Point.X + Dist * FMath::Cos(Angle), Point.Y + Dist * FMath::Sin(Angle));
}


bool FPoissonPoints::IsValidPoint(const FVector2D& Point, const TArray<FVector2D>& Grid, int32 GridWidth, int32 GridHeight)
{
    if (Point.X < 0 || Point.X >= Width || Point.Y < 0 || Point.Y >= Height)
        return false;

    FIntPoint Cell = GetCellCoords(Point);
    int32 SearchStartX = FMath::Max(Cell.X - 2, 0);
    int32 SearchStartY = FMath::Max(Cell.Y - 2, 0);
    int32 SearchEndX = FMath::Min(Cell.X + 2, GridWidth - 1);
    int32 SearchEndY = FMath::Min(Cell.Y + 2, GridHeight - 1);

    for (int32 x = SearchStartX; x <= SearchEndX; x++)
    {
        for (int32 y = SearchStartY; y <= SearchEndY; y++)
        {
            FVector2D Neighbor = Grid[y * GridWidth + x];
            if (Neighbor != FVector2D::ZeroVector)
            {
                float Dist = FVector2D::Distance(Point, Neighbor);
                if (Dist < Radius)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

FIntPoint FPoissonPoints::GetCellCoords(const FVector2D& Point)
{
    return FIntPoint(FMath::FloorToInt(Point.X / CellSize), FMath::FloorToInt(Point.Y / CellSize));
}
