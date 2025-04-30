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
	class USoundBase* BattleMusic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAudioComponent* BattleMusicComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> NextLevel;

	virtual void BeginPlay() override;

	void InitCutScenes();

	void OnLevelStartCutSceneEnd();
	void OnLevelFinishCutSceneEnd();

	UFUNCTION()
	void OnWin();

public:

	void Tick(float DeltaTime) override;

	float GetGameTimeMinutes() const;
};
