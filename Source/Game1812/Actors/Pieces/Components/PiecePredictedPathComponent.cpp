#include "PiecePredictedPathComponent.h"

#include "../Piece.h"
#include "../ScoutPiece.h"
#include "../../../CossacksGameInstance.h"
#include "../../UnitPathArrow.h"
#include "../../HeadQuarters.h"
#include "../../GhostPiece.h"

UPiecePredictedPathComponent::UPiecePredictedPathComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsScout = false;
}

void UPiecePredictedPathComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPiece = Cast<APiece>(GetOwner());

	if (!OwnerPiece)
		return DestroyComponent();

	OwnerPiece->AddOnStartDraggingHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::SpawnArrow));

	OwnerPiece->AddOnMapHitWasDraggedHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::BuildArrow));
	OwnerPiece->AddOnRemovedFromMapHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::DestroyArrow));
	
	OwnerPiece->AddOnOrderAssignHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::ApplyArrow));
	OwnerPiece->AddOnUnitDeathHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::DestroyArrow));

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UPiecePredictedPathComponent::InitStartPoint));
}

void UPiecePredictedPathComponent::InitStartPoint()
{
	AHeadQuarters* hq = AHeadQuarters::GetInstance();

	if (hq)
		PathStartPoint = hq->GetActorLocation() + FVector(0.f, 0.f, 100.f);
}

void UPiecePredictedPathComponent::SpawnArrow()
{
	if (OwnerPiece->IsDead())
		return;

	if (UnitPathArrow.IsValid())
		return;

	const UCossacksGameInstance* gameInstance = GetWorld()->GetGameInstanceChecked<UCossacksGameInstance>();
	UnitPathArrow = GetWorld()->SpawnActor<AUnitPathArrow>(gameInstance->GetUnitPathArrowClass(), FVector::ZeroVector, FRotator::ZeroRotator);

	if (!UnitPathArrow.IsValid())
		return;

	UnitPathArrow->SetStartPoint(PathStartPoint);
}

void UPiecePredictedPathComponent::SpawnGhostPiece(const FVector& Location)
{
	const UCossacksGameInstance* gameInstance = GetWorld()->GetGameInstanceChecked<UCossacksGameInstance>();
	AGhostPiece* ghostPiece = GetWorld()->SpawnActor<AGhostPiece>(gameInstance->GetGhostPieceClass(), Location, FRotator::ZeroRotator);

	if (!ghostPiece)
		return;

	ghostPiece->CopyPiece(OwnerPiece);
	GhostPieces.Add(ghostPiece);
}

void UPiecePredictedPathComponent::BuildArrow()
{
	if (!UnitPathArrow.IsValid())
		SpawnArrow();

	if (!UnitPathArrow.IsValid())
		return;

	if (GhostPieces.IsEmpty()) 
	{
		FVector location = PathStartPoint;
		location.Z = OwnerPiece->GetActorLocation().Z;
		SpawnGhostPiece(location);
	}

	if (bIsScout)
	{
		UnitPathArrow->SetEndPoint(OwnerPiece->GetActorLocation(), false, true);
	}
	else 
	{
		UnitPathArrow->SetEndPoint(OwnerPiece->GetActorLocation(), true);
	}
}

void UPiecePredictedPathComponent::ScoutBuildArrow()
{
	if (!UnitPathArrow.IsValid())
		return;

	UnitPathArrow->SetEndPoint(OwnerPiece->GetActorLocation(), false);
	SpawnGhostPiece(OwnerPiece->GetActorLocation());
}

void UPiecePredictedPathComponent::DestroyArrow()
{
	if (UnitPathArrow.IsValid())
	{
		UnitPathArrow->Destroy();
		UnitPathArrow = nullptr;
	}

	for (AGhostPiece* piece : GhostPieces) 
	{
		piece->Destroy();
	}
	GhostPieces.Empty();
}

void UPiecePredictedPathComponent::ApplyArrow()
{
	DestroyArrow();

	if (!bIsScout)
		PathStartPoint = OwnerPiece->GetActorLocation();
}


void UPiecePredictedPathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPiecePredictedPathComponent::SetScout(bool NewScout)
{
	bIsScout = NewScout;

	AScoutPiece* scoutPiece = Cast<AScoutPiece>(GetOwner());

	if (!scoutPiece)
		return;

	scoutPiece->AddOnOrderPointAddHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::ScoutBuildArrow));
}

