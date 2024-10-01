#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BattleCondition.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleConditionStateChangeDelegate, bool, State);

UINTERFACE(Blueprintable)
class GAME1812_API UConditionsContainer : public UInterface
{
	GENERATED_BODY()
};

class GAME1812_API IConditionsContainer
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<class UBattleCondition*> GetConditions();
};


UCLASS(Abstract, EditInlineNew, CollapseCategories, Blueprintable)
class GAME1812_API UBattleCondition : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText UIText;

	virtual bool Condition(class ABattleObjectivesManager* BattleObjectivesManager);

public:

	UPROPERTY(BlueprintAssignable)
	FOnBattleConditionStateChangeDelegate OnStateChange;

	bool UpdateCondition(class ABattleObjectivesManager* BattleObjectivesManager);

	bool GetState();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FText GetFormattedText() { return UIText; }
};

UCLASS(meta = (DisplayName = "And"))
class GAME1812_API UAndLogicBattleCondition : public UBattleCondition, public IConditionsContainer
{
	GENERATED_BODY()

public:

	UAndLogicBattleCondition();

protected:

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TArray<TObjectPtr<class UBattleCondition>> Conditions;

	bool Condition(class ABattleObjectivesManager* BattleObjectivesManager) override;

	TArray<class UBattleCondition*> GetConditions_Implementation() override { return Conditions; }
};

UCLASS(meta = (DisplayName = "Or"))
class GAME1812_API UOrLogicBattleCondition : public UBattleCondition, public IConditionsContainer
{
	GENERATED_BODY()

public:

	UOrLogicBattleCondition();

protected:

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TArray<TObjectPtr<class UBattleCondition>> Conditions;

	bool Condition(class ABattleObjectivesManager* BattleObjectivesManager) override;

	TArray<class UBattleCondition*> GetConditions_Implementation() override { return Conditions; }
};

UCLASS(meta = (DisplayName = "Timer"))
class GAME1812_API UTimerBattleCondition : public UBattleCondition
{
	GENERATED_BODY()

public:

	UTimerBattleCondition();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Time;

	bool Condition(class ABattleObjectivesManager* BattleObjectivesManager) override;

public:

	FText GetFormattedText() override { return FText::Format(UIText, { Time }); }
};

UCLASS(meta = (DisplayName = "Defeat Enemy"))
class GAME1812_API UDefeatEnemyBattleCondition : public UBattleCondition
{
	GENERATED_BODY()

public:

	UDefeatEnemyBattleCondition();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0, ClampMax = 100))
	float EnemyCasualtyPercentage;

	bool Condition(class ABattleObjectivesManager* BattleObjectivesManager) override;

public:

	FText GetFormattedText() override { return FText::Format(UIText, { EnemyCasualtyPercentage }); }
};

UCLASS(meta = (DisplayName = "Casualties"))
class GAME1812_API UCasualtiesBattleCondition : public UBattleCondition
{
	GENERATED_BODY()

public:

	UCasualtiesBattleCondition();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0, ClampMax = 100))
	float CasualtyPercentage;

	bool Condition(class ABattleObjectivesManager* BattleObjectivesManager) override;

public:

	FText GetFormattedText() override { return FText::Format(UIText, { CasualtyPercentage }); }
};