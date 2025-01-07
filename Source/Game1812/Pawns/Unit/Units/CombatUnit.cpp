#include "CombatUnit.h"

#include "../Orders/UnitOrder.h"
#include "../Orders/FormationMovement.h"

#include "../Components/UnitMovementComponent.h"
#include "../Components/UnitCombatComponent.h"
#include "../Components/UnitReportComponent.h"
#include "../Components/UnitTerrainModifiersComponent.h"
#include "../Components/UnitPieceProjectionComponent.h"
#include "../Controllers/EnemyUnitController.h"
#include "../../../DataAssets/CombatUnitDataAsset.h"

#include "../../../CossacksGameInstance.h"

#include "../../../Actors/Pieces/Piece.h"

ACombatUnit::ACombatUnit()
{
	CombatComponent = CreateDefaultSubobject<UUnitCombatComponent>(TEXT("Combat Component"));

	CombatUnitData = nullptr;
}

void ACombatUnit::BeginPlay()
{
	Super::BeginPlay();

	if (!CombatUnitData)
		return;

	CombatComponent->Init(CombatUnitData->GetCombatUnitStats());
}


void ACombatUnit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Team == ETeam::Russia)
	{
		ReportComponent = NewObject<UUnitReportComponent>(this, TEXT("Report Component"));
		ReportComponent->RegisterComponent();

		if (PieceProjectionComponent)
			PieceProjectionComponent->DestroyComponent();

		PieceProjectionComponent = nullptr;
	}
	else 
	{
		PieceProjectionComponent = NewObject<UUnitPieceProjectionComponent>(this, TEXT("Piece Projection Component"));
		PieceProjectionComponent->RegisterComponent();

		if (ReportComponent)
			ReportComponent->DestroyComponent();

		ReportComponent = nullptr;
	}

	if (!CurrentOrder) 
	{
		CurrentOrder = NewObject<UCombatUnitOrder>();
		CurrentOrder->UnitEnemyReaction = EUnitEnemyReaction::Attack;
	}

	CurrentOrder->Location = GetActorLocation();
}

void ACombatUnit::SpawnDefaultController()
{
	if (GetTeam() == ETeam::France)
	{
		AEnemyUnitController* enemyUnitController = GetWorld()->SpawnActor<AEnemyUnitController>();

		if (enemyUnitController)
			enemyUnitController->Possess(this);
	}
}

void ACombatUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentOrder)
		return;

	if (CurrentOrder->ReorganizationType != EUnitReorganization::None)
	{
		if (CurrentOrder->UnitToReorganizeWith.IsNull()) 
		{
			CurrentOrder->ReorganizationType = EUnitReorganization::None;
			return;
		}	

		if (!IsInReachToReorganize(CurrentOrder->UnitToReorganizeWith))
		{
			MovementComponent->MoveTo(CurrentOrder->UnitToReorganizeWith->GetActorLocation(), EUnitMovementType::Move);
			return;
		}

		if (CurrentOrder->ReorganizationType == EUnitReorganization::Combine) 
		{
			const float morale = CombatComponent->GetMorale();
			const float hp = CombatComponent->GetHP();
			const float otherMorale = CurrentOrder->UnitToReorganizeWith->GetCombatComponent()->GetMorale();
			const float otherHp = CurrentOrder->UnitToReorganizeWith->GetCombatComponent()->GetHP();

			CurrentOrder->UnitToReorganizeWith->GetCombatComponent()->Heal(GetCombatComponent()->GetHP());
			CurrentOrder->UnitToReorganizeWith->GetCombatComponent()->SetMorale((morale * hp + otherMorale * otherHp) / (hp + otherHp) * 0.8f);

			Destroy();
			
			if (OwnerPiece.IsValid())
				OwnerPiece->OnDeathUnit();

			return;
		}

		if (CurrentOrder->ReorganizationType == EUnitReorganization::RedistributeEvenly)
		{
			const float morale = CombatComponent->GetMorale();
			const float hp = CombatComponent->GetHP();
			const float otherMorale = CurrentOrder->UnitToReorganizeWith->GetCombatComponent()->GetMorale();
			const float otherHp = CurrentOrder->UnitToReorganizeWith->GetCombatComponent()->GetHP();
			const float combinedHp = hp + otherHp;
			const float averageHp = combinedHp / 2.0f;

			CombatComponent->SetHP(averageHp);
			CurrentOrder->UnitToReorganizeWith->GetCombatComponent()->SetHP(averageHp);

			if (otherHp - averageHp > 0.0f) 
			{
				CurrentOrder->UnitToReorganizeWith->GetCombatComponent()->SetMorale((morale * hp + otherMorale * (averageHp - otherHp)) / averageHp * 0.8f);
			}
			else 
			{
				CombatComponent->SetMorale((morale * (averageHp - hp) + otherMorale * otherHp) / averageHp * 0.8f);
			}

			CurrentOrder->ReorganizationType = EUnitReorganization::None;
			CurrentOrder->UnitToReorganizeWith = nullptr;
			return;
		}
	}
}

void ACombatUnit::AssignOrder(UUnitOrder* NewOrder)
{
	CurrentOrder = Cast<UCombatUnitOrder>(NewOrder);

	if (!CurrentOrder)
		return;

	if (CurrentOrder->ReorganizationType == EUnitReorganization::None) 
	{
		MovementComponent->ForceMoveTo(CurrentOrder->Location, EUnitMovementType::Move);
		MovementComponent->RotateTo(CurrentOrder->YawRotation);

		if (CurrentOrder->bMoveWithSameSpeed) 
		{
			MovementComponent->SetFormationMovement(CurrentOrder->FormationMovement.Get());
		}
		else 
		{
			MovementComponent->SetFormationMovement(nullptr);
		}

		return;
	}

	if ((CurrentOrder->ReorganizationType == EUnitReorganization::Combine) || (CurrentOrder->ReorganizationType == EUnitReorganization::RedistributeEvenly))
	{
		if (CurrentOrder->UnitToReorganizeWith.IsNull())
			return;

		MovementComponent->ForceMoveTo(CurrentOrder->UnitToReorganizeWith->GetActorLocation(), EUnitMovementType::Move);
		return;
	}
}

void ACombatUnit::SetCombatUnitData(UCombatUnitDataAsset* NewCombatUnitData)
{
	CombatUnitData = NewCombatUnitData;

	if (!CombatUnitData)
		return;

	CombatComponent->Init(CombatUnitData->GetCombatUnitStats());

	OnCombatUnitDataChange();
}

bool ACombatUnit::IsInReachToReorganize(ACombatUnit* OtherUnit)
{
	if (!OtherUnit)
		return false;

	return FVector::DistSquared2D(GetActorLocation(), OtherUnit->GetActorLocation()) < FMath::Pow(10.0f, 2);
}

FCombatUnitStats* ACombatUnit::GetCombatUnitStats() const
{
	return CombatUnitData ? CombatUnitData->GetCombatUnitStats() : nullptr;
}

UUnitOrder* ACombatUnit::GetCurrentOrder()
{
	return CurrentOrder;
}

UUnitMovementComponent* ACombatUnit::GetMovementComponent()
{
	return MovementComponent;
}

float ACombatUnit::GetMovementSpeed() const
{
	return CombatComponent->CalculateMovementSpeed() * GetTerrainModifiers().MovementSpeedModifier;
}

float ACombatUnit::GetRotationSpeed() const
{
	return CombatComponent->CalculateRotationSpeed() * GetTerrainModifiers().RotationSpeedModifier;
}

float ACombatUnit::ApplyDamage(IDamageable* Attacker, float Amount)
{
	return CombatComponent->ApplyDamage(Attacker, Amount);
}

ECombatUnitType ACombatUnit::GetUnitType() const
{
	return GetCombatUnitStats() ? GetCombatUnitStats()->GetUnitType() : ECombatUnitType::None;
}

FVector ACombatUnit::GetLocation()
{
	return GetActorLocation();
}

bool ACombatUnit::IsValidTarget()
{
	return true;
}

float ACombatUnit::GetDefense()
{
	return CombatComponent->CalculateDefense();
}
