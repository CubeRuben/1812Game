#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Pawns/Unit/Orders/UnitOrder.h"
#include "../Pawns/Unit/Orders/AssignedUnitOrder.h"
#include "../Macros/EventDelegate.h"
#include "HeadQuarters.generated.h"

DECLARE_MULTICAST_DELEGATE(FOrdersSendDelegate)

UCLASS()
class GAME1812_API AHeadQuarters : public AActor
{
	GENERATED_BODY()
	
public:	

	AHeadQuarters();

protected:

	static AHeadQuarters* Instance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FAssignedCombatUnitOrder> UnitOrders;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class AAdjutantUnit*> AvailableAdjutants;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int AdjutantsAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RangeForCloseOrders;

	TEMPLATE_EVENT_DELEGATE(FOrdersSendDelegate, OrdersSend);

	virtual void BeginPlay() override;

public:	

	void AddAdjutantUnit(class AAdjutantUnit* AdjutantUnit);
	void RemoveAdjutantUnit(class AAdjutantUnit* AdjutantUnit);

	void AddOrderToAssign(class UCombatUnitOrder* UnitOrder, class ABaseUnit* Unit);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static AHeadQuarters* GetInstance();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HaveAnyOrders();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HaveAnyAdjutants();

	UFUNCTION(BlueprintCallable)
	void SendOrders();

	class ABaseUnit* SpawnUnit(TSubclassOf<class ABaseUnit> UnitClass);
};
