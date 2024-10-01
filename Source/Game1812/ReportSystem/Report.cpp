#include "Report.h"

AReport::AReport()
{
	PrimaryActorTick.bCanEverTick = false;

	PaperMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paper Mesh Component"));
	PaperMeshComponent->SetSimulatePhysics(true);
	PaperMeshComponent->SetUseCCD(true);
	RootComponent = PaperMeshComponent;
}

void AReport::SetReportData(const FUnitReport& ReportData)
{
	Report = ReportData;

	DisplayReportData();
}

void AReport::DisplayReportData_Implementation()
{

}

void AReport::StartDragging()
{
	PaperMeshComponent->SetSimulatePhysics(false);
	SetActorEnableCollision(false);

	FRotator rotation = GetActorRotation();
	rotation.Roll = 0.f;
	rotation.Pitch = 0.f;
	SetActorRotation(rotation);
}

void AReport::StopDragging()
{
	PaperMeshComponent->SetSimulatePhysics(true);
	SetActorEnableCollision(true);
}

void AReport::StartCursorHover()
{
}

void AReport::StopCursorHover()
{
}

void AReport::StartGroupSelectionHover()
{
}

void AReport::StopGroupSelectionHover()
{
}

void AReport::Selected()
{
}

void AReport::SelectionRemoved()
{
}

FVector AReport::GetDragOffset()
{
	return FVector(0, 0, 5.f);
}
