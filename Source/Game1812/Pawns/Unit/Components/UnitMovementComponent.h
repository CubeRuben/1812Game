#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitMovementComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementStartDelegate);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementEndDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveDelegate, float, Distance);

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

	UPROPERTY()
	class UNavigationPath* Path;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Movement")
	int CurrentFollowingSegmentIndex;

	float LastTimeOfMoveAssign;

	FVector GetNextPathPoint();
	FVector GetLastPathPoint();

	FVector ProjectPointToMap(const FVector& Point);

	virtual void BeginPlay() override;

	void MoveAlongPath(float DeltaTime);

	void UpdateMovement(float DeltaTime);

	void RotatePawn(float DeltaTime, float RotationYaw);
	void MovePawn(float DeltaTime, const FVector& Location);

	void UpdatePath();

	void CheckMovementStart();
	void CheckMovementEnd();

public:	

	UPROPERTY(BlueprintAssignable)
	FOnMovementStartDelegate OnMovementStart;

	UPROPERTY(BlueprintAssignable)
	FOnMovementEndDelegate OnMovementEnd;

	FOnMoveDelegate OnMove;

	FVector GetTargetLocation() const;

	bool IsMoving();

	void MoveTo(const FVector& MoveToLocation, bool bForceMove = false);
	void RotateTo(float FinishRotation);
	void StopMoving();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
