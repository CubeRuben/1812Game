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
	class UUnitMovementComponent* GetMovementComponent() override;

	float GetMovementSpeed() const override;
	float GetRotationSpeed() const override;

	class UUnitOrder* GetCurrentOrder();
	void AssignOrder(class UUnitOrder* NewOrder);
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
