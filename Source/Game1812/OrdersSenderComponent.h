#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pawns/Unit/Orders/AssignedUnitOrder.h"
#include "Macros/EventDelegate.h"
#include "OrdersSenderComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOrdersSendDelegate)

UCLASS()
class GAME1812_API UOrdersSenderComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    UOrdersSenderComponent();

protected:

    static UOrdersSenderComponent* Instance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<class AAdjutantUnit*> AvailableAdjutants;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<class ABaseUnit*> ContainedUnit;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int AdjutantsAmount;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FAssignedCombatUnitOrder> UnitOrders;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CloseOrdersRange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool AllowedToSendOrders;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DiscoverRange;

    TEMPLATE_EVENT_DELEGATE(FOrdersSendDelegate, OrdersSend);

    virtual void BeginPlay() override;
    virtual void BeginDestroy() override;

    void SpawnAdjutants();

public:

    UFUNCTION(BlueprintCallable, BlueprintPure)
    static UOrdersSenderComponent* GetInstance() { return Instance; };

    void ReturnAdjutantUnit(class AAdjutantUnit* AdjutantUnit);
    void TakeAdjutantUnit(class AAdjutantUnit* AdjutantUnit);

    void AddContainedUnit(class ABaseUnit* Unit);
    void RemoveContainedUnit(class ABaseUnit* Unit);

    bool IsAllowedToSendOrders() const { return AllowedToSendOrders; }
    void SetAllowedToSendOrders(bool NewAllowedToSendOrders) { AllowedToSendOrders = NewAllowedToSendOrders; }
    float GetDiscoverRange() const { return DiscoverRange; }

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    class ABaseUnit* SpawnUnit(TSubclassOf<class ABaseUnit> UnitClass);
    
    void UpdateFogOfWar();
    
    void AddAssignedOrder(class UCombatUnitOrder* UnitOrder, class ABaseUnit* Unit);

    bool HaveAnyOrders();
    bool HaveAnyAdjutants();

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool CanSendOrders();

    UFUNCTION(BlueprintCallable)
    void SendOrders();

};