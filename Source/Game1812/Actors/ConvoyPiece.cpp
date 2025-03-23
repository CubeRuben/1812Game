#include "ConvoyPiece.h"

#include "../Pawns/Unit/Units/EscortConvoyUnit.h"

AConvoyPiece::AConvoyPiece() :
	Height(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
}

void AConvoyPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Unit.IsValid())
		return;

	if (Unit->IsCoveredInFog() && !bIsCoveredInFog)
	{
		bIsCoveredInFog = true;
		OnCoveredInFogChange(true);
	}
	else if (!Unit->IsCoveredInFog() && bIsCoveredInFog)
	{
		bIsCoveredInFog = false;
		OnCoveredInFogChange(false);
	}

	if (bIsCoveredInFog)
		return;

	const FVector newLocation = FMath::VInterpTo(GetActorLocation(), GetTargetLocation(), DeltaTime, 10.0f);
	SetActorLocation(newLocation);
	SetActorRotation(FRotator(0.0f, Unit->GetActorRotation().Yaw, 0.0f));
}

FVector AConvoyPiece::GetTargetLocation()
{
	FVector targetLocation = Unit->GetActorLocation();
	targetLocation.Z = Height;
	return targetLocation;
}

void AConvoyPiece::SetOwnerUnit(class AEscortConvoyUnit* NewUnit)
{
	Unit = NewUnit;

	if (!Unit.IsValid())
		return;

	SetActorLocation(GetTargetLocation());
}