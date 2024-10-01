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

	virtual void BeginPlay() override;

	void OnActorSpawn(AActor* Actor);

	UFUNCTION()
	void ScanForData();

	UFUNCTION()
	void OnUnitDamageTaken(class ACombatUnit* Unit, float TakenDamage);

public:	

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ABattleObjectivesManager* GetInstance();

	UPROPERTY(BlueprintAssignable)
	FOnBattleObjectivesFinishDelegate OnBattleWin;

	UPROPERTY(BlueprintAssignable)
	FOnBattleObjectivesFinishDelegate OnBattleLost;

	virtual void Tick(float DeltaTime) override;

	float GetStartHP() const;
	float GetTakenCasualties() const;
	float GetEnemyStartHP() const;
	float GetEnemyTakenCasualties() const; 

};
