#include "ReportSpawner.h"

#include "Report.h"

AReportSpawner* AReportSpawner::Instance = nullptr;

AReportSpawner* AReportSpawner::GetInstance()
{
	return Instance;
}

AReportSpawner::AReportSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MaxAmountOfReports = 5;
	ReportsIterator = 0;
	SpawnRotation = 0.f;
	RandomRotation = 25.f;
}

void AReportSpawner::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;
}

void AReportSpawner::AddReport(const FUnitReport& UnitReport)
{
	if (!UnitReport.IsReportValid())
		return;

	const FVector spawnLocation = GetActorLocation();

	const float randomAdditionalRotation = FMath::RandRange(-RandomRotation, RandomRotation);
	const FRotator spawnRotation(0, SpawnRotation + randomAdditionalRotation, 0);

	if (Reports.Num() == MaxAmountOfReports) 
	{
		ReportsIterator %= Reports.Num();

		AReport* report = Reports[ReportsIterator];

		report->SetActorLocation(spawnLocation);
		report->SetActorRotation(spawnRotation);
		report->SetReportData(UnitReport);

		ReportsIterator++;
		return;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AReport* newReport = GetWorld()->SpawnActor<AReport>(ReportClass, spawnLocation, spawnRotation, spawnParams);

	if (!newReport)
		return;

	newReport->SetReportData(UnitReport);
	Reports.Add(newReport);
}
