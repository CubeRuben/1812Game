#include "UnitReportComponent.h"

#include "UnitCombatComponent.h"
#include "../Units/CombatUnit.h"

UUnitReportComponent::UUnitReportComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UUnitReportComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatUnitPawn = Cast<ACombatUnit>(GetOwner());

	if (!CombatUnitPawn)
		return DestroyComponent();
	
	CombatUnitPawn->GetCombatComponent()->OnDamageDealt.AddDynamic(this, &UUnitReportComponent::OnDamageDealt);
	CombatUnitPawn->GetCombatComponent()->OnDamageTaken.AddDynamic(this, &UUnitReportComponent::OnDamageTaken);
	CombatUnitPawn->GetCombatComponent()->OnTemporarilyDefeat.AddDynamic(this, &UUnitReportComponent::OnRetreat);
}

void UUnitReportComponent::OnDamageDealt(ACombatUnit* Unit, float Damage)
{
	Report.AddEnemyLosses(Damage);
}

void UUnitReportComponent::OnDamageTaken(ACombatUnit* Unit, float Damage)
{
	Report.AddAlliedLosses(Damage);
}

void UUnitReportComponent::OnRetreat()
{
	Report.SetHasEverRetreated(true);
}

FUnitReport UUnitReportComponent::RequestUnitReport()
{
	FUnitReport newUnitReport(Report);
	Report.Clear();

	newUnitReport.SetMorale(CombatUnitPawn->GetCombatComponent()->GetMorale());

	return newUnitReport;
}
