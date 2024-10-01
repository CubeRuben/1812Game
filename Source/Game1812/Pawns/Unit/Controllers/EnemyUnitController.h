#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "EnemyUnitController.generated.h"


UCLASS()
class GAME1812_API AEnemyUnitController : public AController
{
	GENERATED_BODY()
	
public:

	AEnemyUnitController();

protected:

	static TArray<AEnemyUnitController*> EnemyControllers;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<class ACombatUnit> CombatUnit;

	UPROPERTY(VisibleAnywhere)
	class UCombatFormation* CombatFormation;

	void BeginPlay() override;
	void BeginDestroy() override;

	void OnPossess(APawn* InPawn) override;

	void FindAlliesInRange(TArray<AEnemyUnitController*> Allies, float Range);

public:

	static const TArray<AEnemyUnitController*>& GetEnemyControllers() { return EnemyControllers; }
	
	class ACombatUnit* GetCombatUnit() const;

	void SetCombatFormation(UCombatFormation* NewCombatFormation) { CombatFormation = NewCombatFormation; }
};
