#include "CutScenePlayer.h"

#include "../UI/CutSceneWidget.h"

#include "MediaSoundComponent.h"
#include "MediaPlayer.h"
#include "MediaSource.h"

#include <Kismet/GameplayStatics.h>

TWeakObjectPtr<ACutScenePlayer> ACutScenePlayer::LevelStartCutScenePlayer = nullptr;
TWeakObjectPtr<ACutScenePlayer> ACutScenePlayer::LevelFinishCutScenePlayer = nullptr;

ACutScenePlayer::ACutScenePlayer() :
	MediaSoundComponent(nullptr),
	CutSceneWidget(nullptr),
	CutSceneMediaPlayer(nullptr),
	CutSceneMediaSource(nullptr),
	Type(ECutSceneType::OnLevelStart)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	MediaSoundComponent = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("Media Sound Component"));
	MediaSoundComponent->bIsUISound = true;
	MediaSoundComponent->PrimaryComponentTick.bTickEvenWhenPaused = true;
	MediaSoundComponent->PrimaryComponentTick.bCanEverTick = true;
	RootComponent = MediaSoundComponent;
}

void ACutScenePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (Type == ECutSceneType::OnLevelStart)
	{
		LevelStartCutScenePlayer = this;
	}
	else if (Type == ECutSceneType::OnLevelFinish) 
	{
		LevelFinishCutScenePlayer = this;
	}
	else if (Type == ECutSceneType::OnGameStart) 
	{
		StartPlaying();
	}
}

void ACutScenePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACutScenePlayer::StartPlaying()
{
	if (!CutSceneMediaPlayer)
		return;

	CutSceneMediaPlayer->OpenSource(CutSceneMediaSource);
	
	if (!CutSceneWidgetClass)
		return;
	
	CutSceneWidget = CreateWidget<UCutSceneWidget>(GetWorld(), CutSceneWidgetClass.Get());

	if (!CutSceneWidget)
		return;

	CutSceneWidget->AddToViewport();
	CutSceneWidget->StartPlaying(this);
	MediaSoundComponent->SetMediaPlayer(CutSceneMediaPlayer);

	CutSceneMediaPlayer->Rewind();
	CutSceneMediaPlayer->Play();

	CutSceneMediaPlayer->OnEndReached.AddDynamic(this, &ACutScenePlayer::StopPlaying);

	APlayerController* const controller = GetWorld()->GetFirstPlayerController();

	if (controller)
		controller->SetPause(true);
}

void ACutScenePlayer::StopPlaying()
{
	if (CutSceneWidget)
		CutSceneWidget->RemoveFromViewport();

	APlayerController* const controller = GetWorld()->GetFirstPlayerController();

	if (controller)
		controller->SetPause(false);

	CutSceneMediaPlayer->Pause();
	CutSceneMediaPlayer->Close();

	OnCutSceneEnd.ExecuteIfBound();
	//Destroy();

	if (Type == ECutSceneType::OnGameStart)
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), NextLevel);
}

