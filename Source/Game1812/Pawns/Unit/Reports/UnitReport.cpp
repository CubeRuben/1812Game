#include "UnitReport.h"

bool FUnitReport::IsReportValid() const
{
	return AlliedLosses > 5.f || EnemyLosses > 5.f || GetAverageMorale() > 0.5f || bHasEverRetreated;
}

void FUnitReport::Clear()
{
	AlliedLosses = 0.f;
	EnemyLosses = 0.f;
	Morale.Empty();
	bHasEverRetreated = false;
}

float FUnitReport::GetAlliedLosses() const
{
	return AlliedLosses;
}

float FUnitReport::GetEnemyLosses() const
{
	return EnemyLosses;
}

float FUnitReport::GetAverageMorale() const
{
	if (Morale.Num() == 0)
		return 0.f;

	float sum = 0.f;

	for (float morale : Morale)
	{
		sum += morale;
	}

	return sum / Morale.Num();
}

bool FUnitReport::GetHasEverRetreated() const
{
	return bHasEverRetreated;
}

void FUnitReport::AddAlliedLosses(float AddedAlliedLosses)
{
	AlliedLosses += AddedAlliedLosses;
}

void FUnitReport::AddEnemyLosses(float AddedEnemyLosses)
{
	EnemyLosses += AddedEnemyLosses;
}

void FUnitReport::SetMorale(float NewMorale)
{
	if (Morale.Num() == 0)
		Morale.Add(NewMorale);

	Morale[0] = NewMorale;
}

void FUnitReport::SetHasEverRetreated(bool NewHasEverRetreated)
{
	bHasEverRetreated = NewHasEverRetreated;
}

FUnitReport& FUnitReport::operator+(const FUnitReport& Other)
{
	AlliedLosses += Other.AlliedLosses;
	EnemyLosses += Other.EnemyLosses;
	
	if (Other.bHasEverRetreated)
		bHasEverRetreated = true;

	Morale.Append(Other.Morale);

	return *this;
}

float UUnitReportFunctionLibrary::GetAverageMorale(const FUnitReport& Other)
{
	return Other.GetAverageMorale();
}
