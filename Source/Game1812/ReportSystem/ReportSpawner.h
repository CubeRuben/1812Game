#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReportSpawner.generated.h"

UCLASS()
class GAME1812_API AReportSpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	AReportSpawner();

protected:

	static AReportSpawner* Instance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AReport> ReportClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class AReport*> Reports;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxAmountOfReports;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int ReportsIterator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RandomRotation;

	virtual void BeginPlay() override;

public:
	
	static AReportSpawner* GetInstance();

	void AddReport(const struct FUnitReport& UnitReport);

};
