#include "PieceMapMarkerComponent.h"

#include "../Piece.h"
#include "../../../CossacksGameInstance.h"

UPieceMapMarkerComponent::UPieceMapMarkerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetWorldScale3D(FVector(0.1f));

	OnDisable();
}

void UPieceMapMarkerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPiece = Cast<APiece>(GetOwner());

	if (!OwnerPiece)
		return DestroyComponent();

	UCossacksGameInstance* gameInstance = Cast<UCossacksGameInstance>(GetWorld()->GetGameInstance());

	if (gameInstance)
		SetMaterial(0, gameInstance->GetPieceMapMarkerMaterial());

	OwnerPiece->AddOnStartDraggingHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceMapMarkerComponent::OnEnable));
	OwnerPiece->AddOnSelectedHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceMapMarkerComponent::OnEnable));

	OwnerPiece->AddOnSelectionRemovedHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceMapMarkerComponent::OnDisable));
	OwnerPiece->AddOnOrderAssignHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceMapMarkerComponent::OnDisable));
	OwnerPiece->AddOnUnitDeathHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceMapMarkerComponent::OnDisable));
}

void UPieceMapMarkerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateLocation();
	UpdateRotation();
}


void UPieceMapMarkerComponent::OnEnable()
{
	if (OwnerPiece->IsDead())
		return;

	SetStaticMesh(OwnerPiece->GetPieceFoundationMeshComponent()->GetStaticMesh());

	SetActive(true);
	SetVisibility(true);
}

void UPieceMapMarkerComponent::OnDisable()
{
	SetVisibility(false);
	SetActive(false);
}

void UPieceMapMarkerComponent::UpdateLocation()
{
	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel(hit, OwnerPiece->GetActorLocation(), OwnerPiece->GetActorLocation() - FVector(0, 0, 4000.f), ECollisionChannel::ECC_GameTraceChannel1);

	SetWorldLocation(hit.Location + FVector(0, 0, 10.f));
}

void UPieceMapMarkerComponent::UpdateRotation()
{
	SetWorldRotation(OwnerPiece->GetResetRotation());
}