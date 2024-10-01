#include "PiecesReturner.h"

#include "../Pieces/Piece.h"

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
	if (!Cast<APiece>(OtherActor))
		return;

	FVector offset = FMath::VRand();
	offset.Z = 0;
	offset *= RandomOffsetRadius;
	OtherActor->SetActorLocation(ReturnPoint->GetComponentLocation() + offset);

	FRotator rotation = OtherActor->GetActorRotation();
	rotation.Roll = 0.f;
	rotation.Pitch = 0.f;
	OtherActor->SetActorRotation(rotation);

	OtherComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
	OtherComp->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
}
