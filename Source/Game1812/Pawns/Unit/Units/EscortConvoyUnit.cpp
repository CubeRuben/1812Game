#include "EscortConvoyUnit.h"

#include "../Components/UnitMovementComponent.h"
#include "../../../Actors/Tools/EscortConvoyFollowPoint.h"
#include "../../../Actors/ConvoyPiece.h"
#include "../../../CossacksGameInstance.h"

AEscortConvoyUnit::AEscortConvoyUnit() :
	CurrentFollowPointIndex(0), HealthPoints(1500.f), Defense(25.0f), MovementSpeed(5.0f), RotationSpeed(5.0f), StartTimeout(5.0f), PathFinished(false)
{

}

void AEscortConvoyUnit::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEscortConvoyUnit::StartMovement, StartTimeout, false);

	MovementComponent->OnMovementEnd.AddDynamic(this, &AEscortConvoyUnit::OnMovementEnd);

	SpawnProjectionPiece();
}

void AEscortConvoyUnit::SpawnProjectionPiece()
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UCossacksGameInstance* const gameInstance = GetWorld()->GetGameInstance<UCossacksGameInstance>();

	if (!gameInstance)
		return;

	AConvoyPiece* const convoyPiece = GetWorld()->SpawnActor<AConvoyPiece>(gameInstance->GetEscortConvoyPieceClass(), params);

	if (convoyPiece)
		convoyPiece->SetOwnerUnit(this);
}

void AEscortConvoyUnit::StartMovement()
{
	TimerHandle.Invalidate();

	MoveToPoint(0);
}

void AEscortConvoyUnit::OnMovementEnd()
{
	if (!FollowPoints.IsValidIndex(CurrentFollowPointIndex))
		return;
	
	const FVector unitLocation = GetActorLocation();
	const FVector followPointLocation = FollowPoints[CurrentFollowPointIndex]->GetActorLocation();

	if (FVector::DistSquared2D(unitLocation, followPointLocation) > 25.0f)
	{
		MoveToPoint(CurrentFollowPointIndex);
		return;
	}

	FollowPoints[CurrentFollowPointIndex]->OnConvoyArrived();
	MoveToPoint(CurrentFollowPointIndex + 1);
}

float AEscortConvoyUnit::ApplyDamage(IDamageable* Attacker, float Amount)
{
	HealthPoints -= Amount;

	if (HealthPoints > 0.0f)
		return 0.0f;

	OnConvoyDeathEvent.Broadcast();
	Destroy();

	return 0.0f;
}

void AEscortConvoyUnit::MoveToPoint(int Index)
{
	if (!FollowPoints.IsValidIndex(Index))
	{
		PathFinished = true;
		OnConvoyMovementFinishEvent.Broadcast();
		return;
	}

	CurrentFollowPointIndex = Index;
	MovementComponent->ForceMoveTo(FollowPoints[Index]->GetActorLocation(), EUnitMovementType::Move);
}
