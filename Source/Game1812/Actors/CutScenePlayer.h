#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CutScenePlayer.generated.h"

DECLARE_DELEGATE(FOnCutSceneEndDelegate);

UENUM()
enum class ECutSceneType : uint8
{
	OnLevelStart,
	OnLevelFinish,
	OnGameStart
};

UCLASS()
class GAME1812_API ACutScenePlayer : public AActor
{
	GENERATED_BODY()
	
public:	

	ACutScenePlayer();

protected:

	static TWeakObjectPtr<ACutScenePlayer> LevelStartCutScenePlayer;
	static TWeakObjectPtr<ACutScenePlayer> LevelFinishCutScenePlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMediaSoundComponent* MediaSoundComponent; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UCutSceneWidget> CutSceneWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCutSceneWidget* CutSceneWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMediaPlayer* CutSceneMediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMediaSource* CutSceneMediaSource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECutSceneType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> NextLevel;

	virtual void BeginPlay() override;

public:	

	FOnCutSceneEndDelegate OnCutSceneEnd;

	static ACutScenePlayer* GetLevelStartCutScenePlayer() { return LevelStartCutScenePlayer.Get(); }
	static ACutScenePlayer* GetLevelFinishCutScenePlayer() { return LevelFinishCutScenePlayer.Get(); }

	virtual void Tick(float DeltaTime) override;

	void StartPlaying();

	UFUNCTION(BlueprintCallable)
	void StopPlaying();

};
