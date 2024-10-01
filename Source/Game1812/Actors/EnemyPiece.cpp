#include "EnemyPiece.h"

#include "../Pawns/Unit/Units/CombatUnit.h"
#include "../DataAssets/CombatUnitDataAsset.h"

AEnemyPiece::AEnemyPiece()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	FoundationMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foundation Mesh"));
	FoundationMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FoundationMeshComponent->SetupAttachment(RootComponent);

	FigureMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Figure Mesh"));
	FigureMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FigureMeshComponent->SetupAttachment(RootComponent);

	Height = 1226.0f;

	bIsCoveredInFog = true;
}

void AEnemyPiece::Tick(float DeltaTime)
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

FVector AEnemyPiece::GetTargetLocation()
{
	FVector targetLocation = Unit->GetActorLocation();
	targetLocation.Z = Height;
	return targetLocation;
}

void AEnemyPiece::OnUnitDestroy(AActor* Actor)
{
	Destroy();
}

void AEnemyPiece::SetCombatUnit(ACombatUnit* NewUnit)
{
	Unit = NewUnit;

	if (!Unit.IsValid())
		return;

	Unit->OnDestroyed.AddDynamic(this, &AEnemyPiece::OnUnitDestroy);

	if (!Unit->GetCombatUnitData())
		return;

	FoundationMeshComponent->SetStaticMesh(Unit->GetCombatUnitData()->GetPieceFoundationMesh());
	FigureMeshComponent->SetStaticMesh(Unit->GetCombatUnitData()->GetPieceMesh());

	SetActorLocation(GetTargetLocation());
}