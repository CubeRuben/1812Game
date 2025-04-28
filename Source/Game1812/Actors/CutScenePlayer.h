#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CutScenePlayer.generated.h"

UENUM()
enum class ECutSceneType : uint8
{
	OnLevelStart,
	OnLevelFinish
};

UCLASS()
class GAME1812_API ACutScenePlayer : public AActor
{
	GENERATED_BODY()
	
public:	

	ACutScenePlayer();

protected:

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

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void StartPlaying();

	UFUNCTION(BlueprintCallable)
	void StopPlaying();

};
