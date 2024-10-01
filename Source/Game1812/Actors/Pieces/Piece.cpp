#include "Piece.h"

#include "../../Pawns/Unit/Orders/UnitOrder.h"
#include "../../Pawns/Unit/Units/CombatUnit.h"
#include "../../UI/BaseOrderWidget.h"
#include "../../CossacksGameInstance.h"
#include "../HeadQuarters.h"
#include "../PaperMap.h"
#include "../UnitPathArrow.h"

#include "Components/PieceMapMarkerComponent.h"
#include "Components/PieceOrderWidgetComponent.h"
#include "Components/PiecePredictedPathComponent.h"
#include "Components/PieceOutlineComponent.h"

#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/WidgetComponent.h>

APiece::APiece()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Piece Collision"));
	BoxCollisionComponent->InitBoxExtent(FVector(20.f, 20.f, 60.f));
	BoxCollisionComponent->SetSimulatePhysics(true);
	BoxCollisionComponent->SetNotifyRigidBodyCollision(true);
	BoxCollisionComponent->SetGenerateOverlapEvents(true);
	RootComponent = BoxCollisionComponent;

	PieceFoundationMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Piece Foundation"));
	PieceFoundationMeshComponent->SetupAttachment(BoxCollisionComponent);
	
	PieceFigureMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Piece Figure"));
	PieceFigureMeshComponent->SetupAttachment(BoxCollisionComponent);

	OrderWidgetComponent = CreateDefaultSubobject<UPieceOrderWidgetComponent>(TEXT("Order Widget"));
	OrderWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	OrderWidgetComponent->SetupAttachment(BoxCollisionComponent);

	MapMarkerComponent = CreateDefaultSubobject<UPieceMapMarkerComponent>(TEXT("Map Marker"));

	PredictedPathComponent = CreateDefaultSubobject<UPiecePredictedPathComponent>(TEXT("Path Arrow"));

	OutlineComponent = CreateDefaultSubobject<UPieceOutlineComponent>(TEXT("Outline"));

	bCanSpawnUnit = true;
	bWasDragged = false;
	bIsDead = false;
}

void APiece::BeginPlay()
{
	Super::BeginPlay();

	BoxCollisionComponent->OnComponentHit.AddDynamic(this, &APiece::OnHit);
	BoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APiece::OnEndOverlap);

}

void APiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APiece::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor) 
		return;

	PlaySoundHit(NormalImpulse.SizeSquared());

	APaperMap* map = Cast<APaperMap>(OtherActor);

	if (!map) 
		return;

	HitComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	//HitComponent->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);

	if (bIsDead)
		return;

	OnMapHit.Broadcast();

	if (bWasDragged) 
	{
		bWasDragged = false;
		OnMapHitWasDragged.Broadcast();
	}
	
	if (!bCanSpawnUnit)
		return;
	
	SpawnUnit();
	bCanSpawnUnit = false;
}

void APiece::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherComp->ComponentTags.Contains("MapBorder"))
		return;

	OnRemovedFromMap.Broadcast();
}

void APiece::SpawnUnit()
{
	if (!AHeadQuarters::GetInstance())
		return;
	
	Unit = AHeadQuarters::GetInstance()->SpawnUnit(UnitClass);
	Unit->SetOwnerPiece(this);

	OnSpawnUnit();

	OnUnitSpawn.Broadcast();
}

void APiece::OnSpawnUnit() { }

void APiece::AssignOrder(UUnitOrder* UnitOrder)
{
	OnOrderAssign.Broadcast();
}

FRotator APiece::GetResetRotation()
{
	FRotator rotation = GetActorRotation();
	rotation.Roll = 0.f;
	rotation.Pitch = 0.f;
	return rotation;
}

void APiece::ResetRotation()
{
	SetActorRotation(GetResetRotation());
}

void APiece::OnDeathUnit()
{
	bIsDead = true;

	PlaySoundDeath();

	UCossacksGameInstance* gameInstance = GetWorld()->GetGameInstance<UCossacksGameInstance>();

	if (!gameInstance)
		return;

	PieceFoundationMeshComponent->SetMaterial(0, gameInstance->GetDeadPieceMaterial());
	PieceFigureMeshComponent->SetMaterial(0, gameInstance->GetDeadPieceMaterial());

	OnUnitDeath.Broadcast();
}

void APiece::StartDragging()
{
	OnStartDragging.Broadcast();

	PlaySoundStartDragging();

	BoxCollisionComponent->SetSimulatePhysics(false);
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//SetActorEnableCollision(false);

	ResetRotation();
}

void APiece::StopDragging()
{
	OnStopDragging.Broadcast();

	BoxCollisionComponent->SetSimulatePhysics(true);
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//SetActorEnableCollision(true);

	FVector velocity = BoxCollisionComponent->GetPhysicsLinearVelocity();
	velocity.Z = 0;
	BoxCollisionComponent->SetPhysicsLinearVelocity(velocity);

	bWasDragged = true;
}

void APiece::StartCursorHover()
{
	OnStartCursorHover.Broadcast();
}

void APiece::StopCursorHover()
{
	OnStopCursorHover.Broadcast();
}

void APiece::StartGroupSelectionHover()
{
	OnStartGroupSelectionHover.Broadcast();
}

void APiece::StopGroupSelectionHover()
{
	OnStopGroupSelectionHover.Broadcast();
}

void APiece::Selected()
{
	OnSelected.Broadcast();

	PlaySoundSelected();
}

void APiece::SelectionRemoved()
{
	OnSelectionRemoved.Broadcast();
}

FVector APiece::GetDragOffset()
{
	return FVector(0, 0, BoxCollisionComponent->GetScaledBoxExtent().Z);
}

ABaseUnit* APiece::GetUnit()
{
	return Unit.Get();
}