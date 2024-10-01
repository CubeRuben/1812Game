#include "CombatUnit.h"

#include "../Orders/UnitOrder.h"

#include "../Components/UnitMovementComponent.h"
#include "../Components/UnitCombatComponent.h"
#include "../Components/UnitReportComponent.h"
#include "../Components/UnitTerrainModifiersComponent.h"
#include "../Components/UnitPieceProjectionComponent.h"
#include "../Controllers/EnemyUnitController.h"
#include "../../../DataAssets/CombatUnitDataAsset.h"

#include "../../../CossacksGameInstance.h"

TMap<ETeam, TArray<ACombatUnit*>> ACombatUnit::CombatUnits = TMap<ETeam, TArray<ACombatUnit*>>();

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

	CombatUnits.FindOrAdd(Team).Add(this);
}

void ACombatUnit::BeginDestroy()
{
	Super::BeginDestroy();

	CombatUnits.FindOrAdd(Team).Remove(this);
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

UUnitCombatComponent* ACombatUnit::GetCombatComponent()
{
	return CombatComponent;
}

UUnitReportComponent* ACombatUnit::GetReportComponent()
{
	return ReportComponent;
}

void ACombatUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ACombatUnit::AssignOrder(UUnitOrder* NewOrder)
{
	CurrentOrder = Cast<UCombatUnitOrder>(NewOrder);

	if (!CurrentOrder)
		return;

	MovementComponent->MoveTo(CurrentOrder->Location, true);
	MovementComponent->RotateTo(CurrentOrder->YawRotation);
}

void ACombatUnit::SetCombatUnitData(UCombatUnitDataAsset* NewCombatUnitData)
{
	CombatUnitData = NewCombatUnitData;

	if (!CombatUnitData)
		return;

	CombatComponent->Init(CombatUnitData->GetCombatUnitStats());

	OnCombatUnitDataChange();
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
	return GetCombatUnitStats()->GetUnitType();
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
