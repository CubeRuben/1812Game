#include "PiecesReturner.h"

#include "../Pieces/Piece.h"
#include "../../Pawns/Player/PlayerPawn.h"
#include "../../Pawns/Player/Components/PlayerInteractionComponent.h"

#include <Components/BoxComponent.h>

APiecesReturner::APiecesReturner()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetGenerateOverlapEvents(true);
	RootComponent = BoxComponent;

	ReturnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Return Point"));
	ReturnPoint->SetupAttachment(BoxComponent);

	RandomOffsetRadius = 150.f;
}

void APiecesReturner::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APiecesReturner::OnBeginOverlap);
}

void APiecesReturner::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APiece* const piece = Cast<APiece>(OtherActor);

	if (!piece)
		return;

	RemoveFromInteraction(piece);
	ReturnPiece(piece, OtherComp);
}

void APiecesReturner::RemoveFromInteraction(APiece* Piece)
{
	if (!PlayerInteractionComponent.IsValid())
	{
		APlayerPawn* const playerPawn = APlayerPawn::GetInstance();

		if (!playerPawn)
			return;

		PlayerInteractionComponent = playerPawn->GetInteractionComponent();

		if (!PlayerInteractionComponent.IsValid())
			return;
	}

	PlayerInteractionComponent->RemoveInteractable(Piece);
}

void APiecesReturner::ReturnPiece(APiece* Piece, UPrimitiveComponent* PieceRootComponent)
{
	FVector offset = FMath::VRand();
	offset.Z = 0;
	offset *= RandomOffsetRadius;
	Piece->SetActorLocation(ReturnPoint->GetComponentLocation() + offset);

	FRotator rotation = Piece->GetActorRotation();
	rotation.Roll = 0.f;
	rotation.Pitch = 0.f;
	Piece->SetActorRotation(rotation);

	PieceRootComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	PieceRootComponent->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
}
