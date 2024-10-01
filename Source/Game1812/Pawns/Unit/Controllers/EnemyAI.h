#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAI.generated.h"

USTRUCT(Blueprintable)
struct GAME1812_API FFormationUnitOffset
{
	GENERATED_USTRUCT_BODY()

public:

	FFormationUnitOffset() {}
	FFormationUnitOffset(TArray<FVector2D> Offsets) : UnitOffsets(Offsets) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector2D> UnitOffsets;
};

UCLASS(EditInlineNew, CollapseCategories, Blueprintable)
class GAME1812_API UCombatFormation : public UObject
{
	GENERATED_BODY()

public:

	UCombatFormation();

public:

	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<class AEnemyUnitController>> UnitControllers;

	void ValidateControllers();

	FVector GetCenterLocation(bool SkipValidation = false);

	void MoveFormationTo(const FVector& Location, float YawRotation, bool SkipValidation = false);
	void AssembleFormation(float YawRotation);
};

UENUM()
enum class EEnemyAIState : uint8 
{
	PreparingForBattle,
	Attacking
};

UCLASS()
class GAME1812_API AEnemyAI : public AActor
{
	GENERATED_BODY()
	
public:	

	AEnemyAI();

protected:

	static AEnemyAI* Instance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 1, ClampMax = 10))
	int CombatFormationsAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyAIState AIState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveTimeout;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveRotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NextTimeForMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TArray<UCombatFormation*> CombatFormations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FFormationUnitOffset> FormationUnitOffsets;

	class ACossacksGameMode* GameMode;

	virtual void BeginPlay() override;

	void CreateFormationsTimeout();
	void CreateFormations(const TArray<class AEnemyUnitController*>& Controllers, int FormationsAmount, int Iterations);

	float GetRotationToHQ(const FVector& Location);

public:	

	void Tick(float DeltaTime) override;

	static AEnemyAI* GetInstance() { return Instance; }

	const TArray<FFormationUnitOffset>& GetFormationUnitOffsets() const { return FormationUnitOffsets; }
	const FVector2D& GetFormationUnitOffset(int Amount, int Index) const;

};
