#include "TutorialCombatPiecesTrigger.h"

#include "../Actors/Pieces/CombatPiece.h"

#include <Components/BoxComponent.h>

ATutorialCombatPiecesTrigger::ATutorialCombatPiecesTrigger()
{
	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Component"));
	TriggerComponent->SetCollisionProfileName("Trigger");
	RootComponent = TriggerComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetRenderCustomDepth(true);
	StaticMeshComponent->SetCustomDepthStencilValue(12);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATutorialCombatPiecesTrigger::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->SetVisibility(false);
	
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ATutorialCombatPiecesTrigger::OnTriggerBeginOverlap);
	TriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ATutorialCombatPiecesTrigger::OnTriggerEndOverlap);
}

void ATutorialCombatPiecesTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetTriggerActive())
		return;

	ACombatPiece* piece = Cast<ACombatPiece>(OtherActor);

	if (piece)
		OnPieceOverlapBeginEvent.Broadcast(piece);
}

void ATutorialCombatPiecesTrigger::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!GetTriggerActive())
		return;

	ACombatPiece* piece = Cast<ACombatPiece>(OtherActor);

	if (piece)
		OnPieceOverlapEndEvent.Broadcast(piece);
}

bool ATutorialCombatPiecesTrigger::GetTriggerActive() const
{
	return StaticMeshComponent->IsVisible();
}

void ATutorialCombatPiecesTrigger::SetTriggerActive(bool NewActive)
{
	StaticMeshComponent->SetVisibility(NewActive);
}
