#include "PieceCombatOrderWidgetComponent.h"

#include "../Piece.h"
#include "../../../DataAssets/CombatUnitDataAsset.h"
#include "../../../Pawns/Unit/Orders/UnitOrder.h"
#include "../../../Pawns/Unit/Units/CombatUnit.h"
#include "../../../Pawns/Unit/Components/UnitCombatComponent.h"
#include "../../../Pawns/Player/PlayerPawn.h"
#include "../../../Pawns/Player/Components/PlayerInteractionComponent.h"

UPieceCombatOrderWidgetComponent::UPieceCombatOrderWidgetComponent()
{

}

void UPieceCombatOrderWidgetComponent::DefaultOrder(UCombatUnitOrder* CombatUnitOrder)
{
	OwnerPiece->AssignOrder(CombatUnitOrder);

	APlayerPawn* playerPawn = APlayerPawn::GetInstance();

	if (!playerPawn)
		return;

	const TArray<AActor*> group = playerPawn->GetInteractionComponent()->GetSelectedGroup();

	if (!group.Contains(OwnerPiece))
		return;

	for (AActor* actor : group)
	{
		if (actor == OwnerPiece)
			continue;

		APiece* piece = Cast<APiece>(actor);

		if (piece)
			piece->AssignOrder(DuplicateObject(CombatUnitOrder, this));
	}
}

void UPieceCombatOrderWidgetComponent::CombineOrder(UCombatUnitOrder* CombatUnitOrder)
{
	TArray<ACombatUnit*> combatUnits;

	GetSelectedCombatUnits(combatUnits);

	for (int a = 0; a < combatUnits.Num(); a++) 
	{
		ACombatUnit* unit1 = combatUnits[a];

		for (int b = combatUnits.Num() - 1; b > a; b--)
		{
			ACombatUnit* unit2 = combatUnits[b];

			if (unit1->GetCombatUnitData() != unit2->GetCombatUnitData())
				continue;

			if (unit1->GetCombatComponent()->GetHealthPoints() + unit2->GetCombatComponent()->GetHealthPoints() > unit2->GetCombatUnitData()->GetCombatUnitStats()->GetBaseHP())
				continue;
		
			UCombatUnitOrder* newCombatUnitOrder = DuplicateObject(CombatUnitOrder, this);

			newCombatUnitOrder->UnitToReorganizeWith = unit2;

			newCombatUnitOrder->bMoveWithSameSpeed = false;
			newCombatUnitOrder->FormationMovement = nullptr;

			unit1->GetOwnerPiece()->AssignOrder(newCombatUnitOrder);

			combatUnits.RemoveAt(b);
			combatUnits.RemoveAt(a);
			a--;
			break;
		}
	}
}

void UPieceCombatOrderWidgetComponent::RedistributeOrder(UCombatUnitOrder* CombatUnitOrder)
{
	TArray<ACombatUnit*> combatUnits;

	GetSelectedCombatUnits(combatUnits);

	struct FHealthAndCounter 
	{
		float Health;
		int Counter;

		float GetAverageHealth() 
		{
			return Health / Counter;
		}
	};

	TMap<int, FHealthAndCounter> averageHealths;

	for (auto& unit : combatUnits) 
	{
		FHealthAndCounter& value = averageHealths.FindOrAdd(unit->GetCombatUnitData()->GetUniqueID());
		value.Health += unit->GetCombatComponent()->GetHealthPoints();
		value.Counter++;
	}

	for (int a = 0; a < combatUnits.Num(); a++)
	{
		ACombatUnit* unit1 = combatUnits[a];
		float averageHealth = averageHealths.FindRef(unit1->GetCombatUnitData()->GetUniqueID()).GetAverageHealth();

		if (unit1->GetCombatComponent()->GetHealthPoints() > averageHealth)
			continue;

		for (int b = combatUnits.Num() - 1; b > a; b--)
		{	
			ACombatUnit* unit2 = combatUnits[b];

			if (unit1->GetCombatUnitData() != unit2->GetCombatUnitData())
				continue;

			if (unit2->GetCombatComponent()->GetHealthPoints() < averageHealth)
				break;

			UCombatUnitOrder* newCombatUnitOrder = DuplicateObject(CombatUnitOrder, this);

			newCombatUnitOrder->UnitToReorganizeWith = unit2;

			newCombatUnitOrder->bMoveWithSameSpeed = false;
			newCombatUnitOrder->FormationMovement = nullptr;

			unit1->GetOwnerPiece()->AssignOrder(newCombatUnitOrder);

			combatUnits.RemoveAt(b);
			combatUnits.RemoveAt(a);
			a--;
			break;
		}
	}
}

void UPieceCombatOrderWidgetComponent::GetSelectedCombatUnits(TArray<ACombatUnit*>& OutArray)
{
	APlayerPawn* playerPawn = APlayerPawn::GetInstance();

	if (!playerPawn)
		return;

	const TArray<AActor*> group = playerPawn->GetInteractionComponent()->GetSelectedGroup();

	for (auto& actor : group)
	{
		APiece* piece = Cast<APiece>(actor);

		if (!piece)
			continue;

		ACombatUnit* combatUnit = Cast<ACombatUnit>(piece->GetUnit());

		if (!combatUnit)
			continue;

		OutArray.AddUnique(combatUnit);
	}

	ACombatUnit* ownerCombatUnit = Cast<ACombatUnit>(OwnerPiece->GetUnit());

	OutArray.AddUnique(ownerCombatUnit);

	OutArray.Sort([](const ACombatUnit& el1, const ACombatUnit& el2) { return el1.GetCombatComponent()->GetHealthPoints() < el2.GetCombatComponent()->GetHealthPoints(); });
}

void UPieceCombatOrderWidgetComponent::AssignOrder(UUnitOrder* UnitOrder)
{
	if (!UnitOrder)
		return;

	UCombatUnitOrder* combatUnitOrder = Cast<UCombatUnitOrder>(UnitOrder);

	if (!combatUnitOrder)
		return;

	switch (combatUnitOrder->ReorganizationType)
	{
	case EUnitReorganization::None:
		DefaultOrder(combatUnitOrder);
		break;
	case EUnitReorganization::Combine:
		CombineOrder(combatUnitOrder);
		break;
	case EUnitReorganization::RedistributeEvenly:
		RedistributeOrder(combatUnitOrder);
		break;
	}

	APlayerPawn* playerPawn = APlayerPawn::GetInstance();

	if (playerPawn)
		playerPawn->GetInteractionComponent()->SetCurrentSelected(nullptr);
}
