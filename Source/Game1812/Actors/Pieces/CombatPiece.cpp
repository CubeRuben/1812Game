#include "CombatPiece.h"

#include "Components/PieceCombatStatsComponent.h"
#include "../../Pawns/Unit/Units/CombatUnit.h"
#include "../../DataAssets/CombatUnitDataAsset.h"
#include "../../CossacksGameInstance.h"
#include "../HeadQuarters.h"

#include <Components/BoxComponent.h>

ACombatPiece::ACombatPiece()
{
	StatsComponent = CreateDefaultSubobject<UPieceCombatStatsComponent>(TEXT("Combat Stats Display"));
}

void ACombatPiece::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatPiece::OnSpawnUnit()
{
	Super::OnSpawnUnit();

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

	UCombatUnitOrder* combatUnitOrder = Cast<UCombatUnitOrder>(UnitOrder);

	if (!combatUnitOrder)
		return;

	combatUnitOrder->Location = GetActorLocation();
	combatUnitOrder->YawRotation = GetActorRotation().Yaw;

	if (AHeadQuarters::GetInstance() && Unit.IsValid())
		AHeadQuarters::GetInstance()->AddOrderToAssign(combatUnitOrder, Unit.Get());
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
