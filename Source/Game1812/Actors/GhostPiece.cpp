#include "GhostPiece.h"

#include "Pieces/Piece.h"

AGhostPiece::AGhostPiece()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	FoundationMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foundation Mesh"));
	FoundationMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FoundationMeshComponent->SetupAttachment(RootComponent);
	
	FigureMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Figure Mesh"));
	FigureMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FigureMeshComponent->SetupAttachment(RootComponent);
}


void AGhostPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGhostPiece::CopyPiece(APiece* Piece)
{
	if (!Piece)
		return;

	const UStaticMeshComponent* pieceFoundation = Piece->GetPieceFoundationMeshComponent();
	const UStaticMeshComponent* figureFoundation = Piece->GetPieceFigureMeshComponent();

	FoundationMeshComponent->SetStaticMesh(pieceFoundation->GetStaticMesh());
	FigureMeshComponent->SetStaticMesh(figureFoundation->GetStaticMesh());

	AddActorWorldOffset(pieceFoundation->GetRelativeLocation());
	SetActorRotation(FRotator(0.f, Piece->GetActorRotation().Yaw, 0.f));
}
