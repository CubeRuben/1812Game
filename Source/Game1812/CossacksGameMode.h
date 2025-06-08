#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CossacksGameMode.generated.h"

UCLASS()
class GAME1812_API ACossacksGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ACossacksGameMode();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GameTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GameMinutesPerRealSecond;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* BattleStartSFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> NextLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WinWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> LostWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UUserWidget* BattleFinishWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText BattleTip;

	virtual void BeginPlay() override;

	void InitCutScenes();

	void OnLevelStartCutSceneEnd();
	void OnLevelFinishCutSceneEnd();

	UFUNCTION()
	void OnWin();

	UFUNCTION()
	void OnLost();

public:

	void Tick(float DeltaTime) override;

	float GetGameTimeSeconds() const;
	float GetGameTimeMinutes() const;
	float GetGameTimeHours() const;

	UFUNCTION(BlueprintCallable)
	void GoToNextLevel();
};
