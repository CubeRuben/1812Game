#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../Macros/EventDelegate.h"
#include "UnitMovementComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementStartDelegate);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementEndDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveDelegate, float, Distance);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovementEndStaticDelegate, class ABaseUnit*);

UENUM()
enum class EUnitMovementType : uint8 
{
	Move,
	Attack
};

UCLASS(Blueprintable, BlueprintType)
class GAME1812_API UUnitMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UUnitMovementComponent();

protected:

	class ABaseUnit* UnitPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Movement")
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Movement")
	bool bMustRotateToTargetRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Movement")
	float TargetRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Movement")
	bool bIsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Movement")
	TObjectPtr<class UFormationMovement> FormationMovement;

	UPROPERTY()
	class UNavigationPath* Path;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Movement")
	int CurrentFollowingSegmentIndex;

	float LastTimeOfMoveAssign;

	TEMPLATE_GLOBAL_EVENT_DELEGATE(FOnMovementEndStaticDelegate, MovementEnd);

	FVector GetNextPathPoint();
	FVector GetNextPathPoint(const FVector& ActorLocation);
	FVector GetLastPathPoint();

	FVector ProjectPointToMap(const FVector& Point);

	virtual void BeginPlay() override;

	void MoveAlongPath(float DeltaTime);

	float UpdateMovement(float DeltaTime);

	void RotatePawn(float DeltaTime, float RotationYaw);

	void UpdatePath();

	void CheckMovementStart();
	void CheckMovementEnd();

	float GetMovementSpeed();
	float GetRotationSpeed();

public:	

	UPROPERTY(BlueprintAssignable)
	FOnMovementStartDelegate OnMovementStart;

	UPROPERTY(BlueprintAssignable)
	FOnMovementEndDelegate OnMovementEnd;

	FOnMoveDelegate OnMove;

	FVector GetTargetLocation() const;

	void SetFormationMovement(class UFormationMovement* NewFormationMovement);

	bool IsMoving() const;

	void MoveTo(const FVector& MoveToLocation, EUnitMovementType MovementType);
	void ForceMoveTo(const FVector& MoveToLocation, EUnitMovementType MovementType);
	void RotateTo(float FinishRotation);
	void StopMoving();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
