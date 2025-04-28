#include "CutScenePlayer.h"

#include "../UI/CutSceneWidget.h"

#include "MediaSoundComponent.h"
#include "MediaPlayer.h"
#include "MediaSource.h"

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
	RootComponent = MediaSoundComponent;
}

void ACutScenePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (Type == ECutSceneType::OnLevelStart)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACutScenePlayer::StartPlaying);
		return;
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

	CutSceneMediaPlayer->Rewind();

	CutSceneWidget->AddToViewport();
	CutSceneWidget->StartPlaying(this);
	MediaSoundComponent->SetMediaPlayer(CutSceneMediaPlayer);

	CutSceneMediaPlayer->Play();

	CutSceneMediaPlayer->OnEndReached.AddDynamic(this, &ACutScenePlayer::StopPlaying);

	APlayerController* const controller = GetWorld()->GetFirstPlayerController();

	if (!controller)
		return;

	controller->SetPause(true);
}

void ACutScenePlayer::StopPlaying()
{
	if (CutSceneWidget)
		CutSceneWidget->RemoveFromViewport();

	APlayerController* const controller = GetWorld()->GetFirstPlayerController();

	if (controller)
		controller->SetPause(false);

	Destroy();
}

