#include "CombatPiece.h"

#include "Components/PieceCombatStatsComponent.h"
#include "Components/PieceCombatOrderWidgetComponent.h"
#include "../../Pawns/Unit/Units/CombatUnit.h"
#include "../../Pawns/Unit/Orders/UnitOrder.h"
#include "../../DataAssets/CombatUnitDataAsset.h"
#include "../../CossacksGameInstance.h"
#include "../../OrdersSenderComponent.h"

#include <Components/BoxComponent.h>

ACombatPiece::ACombatPiece()
{
	StatsComponent = CreateDefaultSubobject<UPieceCombatStatsComponent>(TEXT("Combat Stats Display"));

	CombatOrderWidgetComponent = CreateDefaultSubobject<UPieceCombatOrderWidgetComponent>(TEXT("Order Widget"));
	CombatOrderWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	CombatOrderWidgetComponent->SetupAttachment(BoxCollisionComponent);
}

void ACombatPiece::SpawnUnit()
{
	Super::SpawnUnit();

	ACombatUnit* combatUnit = Cast<ACombatUnit>(Unit);

	if (combatUnit)
		combatUnit->SetCombatUnitData(CombatUnitData);
}

void ACombatPiece::UpdatePieceMesh()
{
	if (!CombatUnitData)
		return;

	PieceFoundationMeshComponent->SetStaticMesh(CombatUnitData->GetPieceFoundationMesh());
	PieceFigureMeshComponent->SetStaticMesh(CombatUnitData->GetPieceMesh());
}

void ACombatPiece::AssignOrder(UUnitOrder* UnitOrder)
{
	Super::AssignOrder(UnitOrder);

	UCombatUnitOrder* const combatUnitOrder = Cast<UCombatUnitOrder>(UnitOrder);

	if (!combatUnitOrder)
		return;

	combatUnitOrder->Location = GetActorLocation();
	combatUnitOrder->YawRotation = GetActorRotation().Yaw;

	UOrdersSenderComponent* const ordersSender = UOrdersSenderComponent::GetInstance();

	if (ordersSender && Unit.IsValid())
		ordersSender->AddAssignedOrder(combatUnitOrder, Unit.Get());
}

int ACombatPiece::GetPriority() const
{
	return CombatUnitData ? CombatUnitData->GetFormationSortPriority() : 0;
}

UCombatUnitDataAsset* ACombatPiece::GetCombatUnitData() const
{
	return CombatUnitData;
}

void ACombatPiece::SetCombatUnitData(UCombatUnitDataAsset* NewCombatUnitData)
{
	CombatUnitData = NewCombatUnitData;

	UpdatePieceMesh();
}
