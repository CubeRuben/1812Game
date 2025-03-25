#include "PiecePredictedPathComponent.h"

#include "../Piece.h"
#include "../ScoutPiece.h"
#include "../../../CossacksGameInstance.h"
#include "../../../OrdersSenderComponent.h"
#include "../../UnitPathArrow.h"
#include "../../GhostPiece.h"

UPiecePredictedPathComponent::UPiecePredictedPathComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsScout = false;
	PathStartPoint = FVector::ZeroVector;
}

void UPiecePredictedPathComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPiece = Cast<APiece>(GetOwner());

	if (!OwnerPiece)
		return DestroyComponent();

	OwnerPiece->AddOnStartDraggingHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::SpawnArrow));

	OwnerPiece->AddOnMapHitWasDraggedHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::BuildArrow));
	OwnerPiece->AddOnRemovedFromMapHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::DestroyArrow));
	
	OwnerPiece->AddOnOrderAssignHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::ApplyArrow));
	OwnerPiece->AddOnUnitDeathHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::DestroyArrow));
}

void UPiecePredictedPathComponent::SpawnArrow()
{
	if (OwnerPiece->IsDead())
		return;

	if (UnitPathArrow.IsValid())
		return;

	const UCossacksGameInstance* gameInstance = GetWorld()->GetGameInstanceChecked<UCossacksGameInstance>();
	UnitPathArrow = GetWorld()->SpawnActor<AUnitPathArrow>(gameInstance->GetUnitPathArrowClass(), FVector::ZeroVector, FRotator::ZeroRotator);

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

	if (bIsScout)
	{
		SetStartPointAtOrdersSender();

		UnitPathArrow->SetStartPoint(PathStartPoint);
		UnitPathArrow->SetEndPoint(OwnerPiece->GetActorLocation(), false, true);
	}
	else 
	{
		if (PathStartPoint.IsNearlyZero())
			SetStartPointAtOrdersSender();

		UnitPathArrow->SetStartPoint(PathStartPoint);
		UnitPathArrow->SetEndPoint(OwnerPiece->GetActorLocation(), true);
	}

	if (GhostPieces.IsEmpty())
	{
		FVector location = PathStartPoint;
		location.Z = OwnerPiece->GetActorLocation().Z;
		SpawnGhostPiece(location);
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

void UPiecePredictedPathComponent::SetStartPointAtOrdersSender()
{
	UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

	if (ordersSender)
		PathStartPoint = ordersSender->GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, 100.f);
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

	scoutPiece->AddOnOrderPointAddHandler(FPieceEventDelegate::FDelegate::CreateUObject(this, &UPiecePredictedPathComponent::ScoutBuildArrow));
}

