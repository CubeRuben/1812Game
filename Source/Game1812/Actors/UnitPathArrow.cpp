#include "UnitPathArrow.h"

#include <Components/SplineComponent.h>
#include <Components/SplineMeshComponent.h>

#include <NavigationPath.h>
#include <NavigationSystem.h>


AUnitPathArrow::AUnitPathArrow()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	SplinePathComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Path Component"));
	SplinePathComponent->SetupAttachment(RootComponent);

	SegmentLength = 100.0f;
	ArrowLocationHeight = 0.f;
}

void AUnitPathArrow::BeginPlay()
{
	Super::BeginPlay();

	FVector location = GetActorLocation();

	location.Z = ArrowLocationHeight;

	SetActorLocation(location);
}

void AUnitPathArrow::BuildArrow()
{
	for (USplineMeshComponent* splineMeshComponent : SplineMeshComponents) 
	{
		splineMeshComponent->DestroyComponent();
	}
	SplineMeshComponents.Empty();

	const float splineLength = SplinePathComponent->GetSplineLength();
	const float middleSplineLength = splineLength - (2 * SegmentLength);
	const int amountOfSegments = (int)(middleSplineLength / SegmentLength);
	const float middleSegmentLength = middleSplineLength / amountOfSegments;

	for (int i = 0; i < amountOfSegments + 1; i++) 
	{
		USplineMeshComponent* splineMeshComponent = NewObject<USplineMeshComponent>(this);
		splineMeshComponent->SetCastShadow(false);

		float segmentLength = SegmentLength;

		if (i == 0) 
		{
			splineMeshComponent->SetStaticMesh(ArrowOutlineEndStaticMesh);
		}
		else if (i == amountOfSegments) 
		{
			splineMeshComponent->SetStaticMesh(ArrowOutlineStartStaticMesh);
		}
		else 
		{
			splineMeshComponent->SetStaticMesh(ArrowOutlineMiddleStaticMesh);
			segmentLength = middleSegmentLength;
		}
		
		splineMeshComponent->SetMobility(EComponentMobility::Stationary);
		splineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		splineMeshComponent->RegisterComponentWithWorld(GetWorld());
		splineMeshComponent->AttachToComponent(SplinePathComponent, FAttachmentTransformRules::KeepRelativeTransform);
		splineMeshComponent->SetForwardAxis(ESplineMeshAxis::X);
	
		const FVector startPoint = SplinePathComponent->GetLocationAtDistanceAlongSpline(SegmentLength * i, ESplineCoordinateSpace::Local);
		const FVector startTangent = SplinePathComponent->GetTangentAtDistanceAlongSpline(SegmentLength * i, ESplineCoordinateSpace::Local);
		const FVector endPoint = SplinePathComponent->GetLocationAtDistanceAlongSpline(SegmentLength * (i + 1), ESplineCoordinateSpace::Local);
		const FVector endTangent = SplinePathComponent->GetTangentAtDistanceAlongSpline(SegmentLength * (i + 1), ESplineCoordinateSpace::Local);

		const FVector adjustedStartTangent = startTangent.GetSafeNormal() * SegmentLength;
		const FVector adjustedEndTangent = endTangent.GetSafeNormal() * SegmentLength;

		splineMeshComponent->SetStartAndEnd(startPoint, adjustedStartTangent, endPoint, adjustedEndTangent, true);
		
		splineMeshComponent->SetStartScale(FVector2D(1, 1));
		splineMeshComponent->SetEndScale(FVector2D(1, 1));

		SplineMeshComponents.Add(splineMeshComponent);
	}
}

void AUnitPathArrow::GetPathPoints(TArray<FVector>& PointsArray, const FVector& From, const FVector& To)
{
	const FVector start = ProjectPointToMap(From);
	const FVector end = ProjectPointToMap(To);

	UNavigationPath* path = UNavigationSystemV1::FindPathToLocationSynchronously(this, start, end);

	if (path)
		PointsArray.Append(path->PathPoints);
}

void AUnitPathArrow::UpdateSplineWithPath()
{
	SplinePathComponent->ClearSplinePoints();

	TArray<FVector> points = TArray<FVector>(PathPoints);
	points.Append(TempPathPoints);

	for (int i = points.Num() - 1; i >= 0; i--)
	{
		FVector point = points[i];
		point.Z = ArrowLocationHeight;

		SplinePathComponent->AddSplinePoint(point, ESplineCoordinateSpace::World, false);

		const int reverseIter = points.Num() - 1 - i;
		SplinePathComponent->SetTangentAtSplinePoint(reverseIter, FVector::ZeroVector, ESplineCoordinateSpace::World);
	}

	SplinePathComponent->UpdateSpline();
}

FVector AUnitPathArrow::ProjectPointToMap(const FVector& Point)
{
	FHitResult hit;

	GetWorld()->LineTraceSingleByChannel(hit, Point, Point - FVector(0, 0, 4000.f), ECollisionChannel::ECC_GameTraceChannel1);

	if (!hit.bBlockingHit)
		return Point;

	return hit.Location;
}

void AUnitPathArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnitPathArrow::SetStartPoint(const FVector& NewStartPoint)
{
	StartPoint = NewStartPoint;
	EndPoint = NewStartPoint;
}

void AUnitPathArrow::SetEndPoint(const FVector& NewEndPoint, bool ClearPoints, bool IsTemp)
{
	TempPathPoints.Empty();

	if (ClearPoints)
		PathPoints.Empty();

	TArray<FVector>& arr = (IsTemp) ? TempPathPoints : PathPoints;

	FVector startPoint = (ClearPoints) ? StartPoint : EndPoint;

	GetPathPoints(arr, startPoint, NewEndPoint);

	if (!IsTemp)
		EndPoint = NewEndPoint;

	UpdateSplineWithPath();

	BuildArrow();
}