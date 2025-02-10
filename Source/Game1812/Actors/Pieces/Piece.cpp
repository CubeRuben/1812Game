#include "Piece.h"

#include "../../Pawns/Unit/Orders/UnitOrder.h"
#include "../../Pawns/Unit/Units/CombatUnit.h"
#include "../../UI/BaseOrderWidget.h"
#include "../../CossacksGameInstance.h"
#include "../HeadQuarters.h"
#include "../PaperMap.h"
#include "../UnitPathArrow.h"

#include "Components/PieceMapMarkerComponent.h"
#include "Components/PiecePredictedPathComponent.h"
#include "Components/PieceOutlineComponent.h"

#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/WidgetComponent.h>

FPieceGlobalEventDelegate APiece::OnMapHitWasDraggedGlobalEvent = FPieceGlobalEventDelegate();
FPieceGlobalEventDelegate APiece::OnOrderAssignGlobalEvent = FPieceGlobalEventDelegate();

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

	APaperMap* map = Cast<APaperMap>(OtherActor);

	if (!map) 
		return;

	HitComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	//HitComponent->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);

	if (bIsDead)
		return;

	OnMapHitEvent.Broadcast();

	if (bWasDragged) 
	{
		bWasDragged = false;
		OnMapHitWasDraggedEvent.Broadcast();
		OnMapHitWasDraggedGlobalEvent.ExecuteIfBound(this);
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

	OnRemovedFromMapEvent.Broadcast();
}

void APiece::SpawnUnit()
{
	if (!AHeadQuarters::GetInstance())
		return;
	
	Unit = AHeadQuarters::GetInstance()->SpawnUnit(UnitClass);
	Unit->SetOwnerPiece(this);

	OnUnitSpawnEvent.Broadcast();
}

void APiece::AssignOrder(UUnitOrder* UnitOrder)
{
	OnOrderAssignEvent.Broadcast();
	OnOrderAssignGlobalEvent.ExecuteIfBound(this);
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

	PlaySound(EPieceSoundType::Death);

	UCossacksGameInstance* gameInstance = GetWorld()->GetGameInstance<UCossacksGameInstance>();

	if (!gameInstance)
		return;

	PieceFoundationMeshComponent->SetMaterial(0, gameInstance->GetDeadPieceMaterial());
	PieceFigureMeshComponent->SetMaterial(0, gameInstance->GetDeadPieceMaterial());

	OnUnitDeathEvent.Broadcast();
}

void APiece::StartDragging()
{
	OnStartDraggingEvent.Broadcast();

	PlaySound(EPieceSoundType::StartDragging);

	BoxCollisionComponent->SetSimulatePhysics(false);
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//SetActorEnableCollision(false);

	ResetRotation();
}

void APiece::StopDragging()
{
	OnStopDraggingEvent.Broadcast();

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
	OnStartCursorHoverEvent.Broadcast();
}

void APiece::StopCursorHover()
{
	OnStopCursorHoverEvent.Broadcast();
}

void APiece::StartGroupSelectionHover()
{
	OnStartGroupSelectionHoverEvent.Broadcast();
}

void APiece::StopGroupSelectionHover()
{
	OnStopGroupSelectionHoverEvent.Broadcast();
}

void APiece::Selected()
{
	OnSelectedEvent.Broadcast();

	PlaySound(EPieceSoundType::Selected);
}

void APiece::SelectionRemoved()
{
	OnSelectionRemovedEvent.Broadcast();
}

FVector APiece::GetDragOffset()
{
	return FVector(0, 0, BoxCollisionComponent->GetScaledBoxExtent().Z);
}

ABaseUnit* APiece::GetUnit()
{
	return Unit.Get();
}