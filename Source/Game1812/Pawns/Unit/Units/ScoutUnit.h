#pragma once

#include "CoreMinimal.h"
#include "../BaseUnit.h"
#include "../Components/Damageable.h"
#include "Image/ImageBuilder.h"
#include "ScoutUnit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FScoutMovementStateDelegate);

using namespace UE::Geometry;

UCLASS()
class GAME1812_API AScoutUnit : public ABaseUnit, public IDamageable
{
	GENERATED_BODY()

public:

	AScoutUnit();

	TQueue<FVector> ExplorationLocations;
	
	UFUNCTION()
	void OnMovementComplete();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UScoutUnitOrder* CurrentOrder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Contained;

	TImageBuilder<FVector4f> DiscoveredArea;

	UPROPERTY(BlueprintAssignable)
	FScoutMovementStateDelegate OnMovementStart;

	UPROPERTY(BlueprintAssignable)
	FScoutMovementStateDelegate OnMovementEnd;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnReturnToHQ();

public:

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float PredictMovementTime();

	//ABaseUnit class override
	float GetMovementSpeed() const override;
	float GetRotationSpeed() const override;

	class UUnitOrder* GetCurrentOrder();
	void AssignOrder(class UUnitOrder* NewOrder);

	void OnBeingAddedToOrdersSender() override;
	void OnBeingRemovedFromOrdersSender() override;

	bool IsAffectedByFog() override { return !Contained; }
	//

	//IDamageable Interface
	float ApplyDamage(IDamageable* Attacker, float Amount) override;
	ETeam GetTeam() const override { return Team; };
	ECombatUnitType GetUnitType() const override;
	FVector GetLocation() override;
	bool IsValidTarget() override;
	float GetDefense() override;
	//
};
