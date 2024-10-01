#include "PieceCombatStatsComponent.h"

#include "../CombatPiece.h"
#include "../../../CossacksGameInstance.h"
#include "../../../Pawns/Unit/Units/CombatUnit.h"
#include "../../../Pawns/Unit/Components/UnitCombatComponent.h"

UPieceCombatStatsComponent::UPieceCombatStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHPRatio = 1.f;
	CurrentMoraleRatio = 1.f;
	UnitHPRatio = 1.f;
	UnitMoraleRatio = 1.f;
}

void UPieceCombatStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPiece = Cast<ACombatPiece>(GetOwner());

	if (!OwnerPiece)
		return DestroyComponent();

	OwnerPiece->AddOnUnitSpawnHandler(FOnPieceChangeDelegate::FDelegate::CreateUObject(this, &UPieceCombatStatsComponent::OnUnitSpawn));
}

void UPieceCombatStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!StatsMaterial)
		return;

	UpdateUnitStats();

	CurrentHPRatio = FMath::FInterpTo(CurrentHPRatio, UnitHPRatio, DeltaTime, 2.f);
	CurrentMoraleRatio = FMath::FInterpTo(CurrentMoraleRatio, UnitMoraleRatio, DeltaTime, 2.f);

	StatsMaterial->SetScalarParameterValue("LeftProgress", 1.f - CurrentHPRatio);
	StatsMaterial->SetScalarParameterValue("RightProgress", 1.f - CurrentMoraleRatio);
}

void UPieceCombatStatsComponent::OnUnitSpawn()
{
	CombatUnit = Cast<ACombatUnit>(OwnerPiece->GetUnit());

	UCossacksGameInstance* gameInstance = GetWorld()->GetGameInstanceChecked<UCossacksGameInstance>();

	StatsMaterial = UMaterialInstanceDynamic::Create(gameInstance->GetCombatStatsPieceMaterial(), this);
	OwnerPiece->GetPieceFoundationMeshComponent()->SetMaterial(0, StatsMaterial);
}

void UPieceCombatStatsComponent::UpdateUnitStats()
{
	if (!CanUpdateStats())
		return;

	UnitHPRatio = CombatUnit->GetCombatComponent()->GetHPRatio();
	UnitMoraleRatio = CombatUnit->GetCombatComponent()->GetMorale();
}

bool UPieceCombatStatsComponent::CanUpdateStats()
{
	if (CombatUnit.IsValid())
		return !CombatUnit->IsCoveredInFog();

	return false;
}

