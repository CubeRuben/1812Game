#pragma once

#include "CoreMinimal.h"
#include "UnitReport.generated.h"

USTRUCT(BlueprintType)
struct GAME1812_API FUnitReport
{

	GENERATED_USTRUCT_BODY();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AlliedLosses = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EnemyLosses = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<float> Morale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHasEverRetreated = false;

public:

	bool IsReportValid() const;

	void Clear();

	float GetAlliedLosses() const;
	float GetEnemyLosses() const;
	float GetAverageMorale() const;
	bool GetHasEverRetreated() const;

	void AddAlliedLosses(float AddedAlliedLosses);
	void AddEnemyLosses(float AddedEnemyLosses);
	void SetMorale(float NewMorale);
	void SetHasEverRetreated(bool NewHasEverRetreated);

	FUnitReport& operator+(const FUnitReport& Other);
};


UCLASS()
class GAME1812_API UUnitReportFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float GetAverageMorale(const FUnitReport& Other);
};