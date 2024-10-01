#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Reports/UnitReport.h"
#include "UnitReportComponent.generated.h"


UCLASS()
class GAME1812_API UUnitReportComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UUnitReportComponent();

protected:

	class ACombatUnit* CombatUnitPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUnitReport Report;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDamageDealt(class ACombatUnit* Unit, float Damage);

	UFUNCTION()
	void OnDamageTaken(class ACombatUnit* Unit, float Damage);

	UFUNCTION()
	void OnRetreat();

public:

	UFUNCTION(BlueprintCallable)
	FUnitReport RequestUnitReport();
};
