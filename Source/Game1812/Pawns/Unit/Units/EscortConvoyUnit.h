#pragma once

#include "CoreMinimal.h"
#include "../BaseUnit.h"
#include "../Components/Damageable.h"
#include "../../../Macros/EventDelegate.h"
#include "EscortConvoyUnit.generated.h"

DECLARE_MULTICAST_DELEGATE(FEscortConvoyDelegate);

#define ADD_ESCORT_CONVOY_EVENT(name) TEMPLATE_EVENT_DELEGATE(FEscortConvoyDelegate, name);

UCLASS()
class GAME1812_API AEscortConvoyUnit : public ABaseUnit, public IDamageable
{
	GENERATED_BODY()
	
public:

	AEscortConvoyUnit();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class AEscortConvoyFollowPoint*> FollowPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CurrentFollowPointIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealthPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StartTimeout;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool PathFinished;

	FTimerHandle TimerHandle;

	ADD_ESCORT_CONVOY_EVENT(ConvoyMovementFinish);
	ADD_ESCORT_CONVOY_EVENT(ConvoyDeath);

	virtual void BeginPlay() override;

	void SpawnProjectionPiece();

	void StartMovement();

	void MoveToPoint(int Index);

	UFUNCTION()
	void OnMovementEnd();

public:

	bool IsDead() const { return HealthPoints <= 0.0f; }
	bool IsPathFinished() const { return PathFinished; }

	//ABaseUnit override
	virtual float GetMovementSpeed() const { return MovementSpeed; }
	virtual float GetRotationSpeed() const { return RotationSpeed; }
	//

	//IDamageable Interface
	float ApplyDamage(IDamageable* Attacker, float Amount) override;

	ETeam GetTeam() const override { return ETeam::Russia; };
	ECombatUnitType GetUnitType() const override { return ECombatUnitType::Artillery; };
	FVector GetLocation() override { return GetActorLocation(); }
	bool IsValidTarget() override { return true; }
	float GetDefense() override { return Defense; }
	//
};
