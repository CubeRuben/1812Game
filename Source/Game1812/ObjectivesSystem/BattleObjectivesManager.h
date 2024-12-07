#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleObjectivesManager.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleObjectivesFinishDelegate);

UENUM(BlueprintType)
enum class EBattleState : uint8
{
	OnGoing,
	Win,
	Lose
};

UCLASS()
class GAME1812_API ABattleObjectivesManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ABattleObjectivesManager();

protected:

	static ABattleObjectivesManager* Instance;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	class UBattleCondition* WinCondition;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	class UBattleCondition* LoseCondition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBattleState BattleState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StartHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TakenCasualties;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EnemyStartHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EnemyTakenCasualties;

	UPROPERTY()
	class ACossacksGameState* GameState;

	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ABattleObjectivesManager* GetInstance();

	UPROPERTY(BlueprintAssignable)
	FOnBattleObjectivesFinishDelegate OnBattleWin;

	UPROPERTY(BlueprintAssignable)
	FOnBattleObjectivesFinishDelegate OnBattleLost;

	void Init(class ACossacksGameState* NewGameState);

	virtual void Tick(float DeltaTime) override;

	float GetStartHP() const { return StartHP; };
	float GetTakenCasualties() const { return TakenCasualties; };
	float GetEnemyStartHP() const { return EnemyStartHP; };
	float GetEnemyTakenCasualties() const { return EnemyTakenCasualties; };

};
