#include "CossacksGameMode.h"

#include "Actors/CutScenePlayer.h"
#include "Actors/MusicManager.h"
#include "ObjectivesSystem/BattleObjectivesManager.h"
#include "Blueprint/UserWidget.h"

#include <Kismet/GameplayStatics.h>
#include <Components/AudioComponent.h>

ACossacksGameMode::ACossacksGameMode() :
	GameTime(0.0f),
	GameMinutesPerRealSecond(5.0f / 60.0f),
	BattleStartSFX(nullptr),

	BattleFinishWidget(nullptr)
{

}

void ACossacksGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACossacksGameMode::InitCutScenes);
}

void ACossacksGameMode::InitCutScenes()
{
	ABattleObjectivesManager* objectivesManager = ABattleObjectivesManager::GetInstance();
	objectivesManager->OnBattleWin.AddDynamic(this, &ACossacksGameMode::OnWin);
	objectivesManager->OnBattleLost.AddDynamic(this, &ACossacksGameMode::OnLost);

	ACutScenePlayer* cutScenePlayer2 = ACutScenePlayer::GetLevelFinishCutScenePlayer();
	if (cutScenePlayer2)
	{
		cutScenePlayer2->OnCutSceneEnd.BindUObject(this, &ACossacksGameMode::OnLevelFinishCutSceneEnd);
	}

	ACutScenePlayer* cutScenePlayer1 = ACutScenePlayer::GetLevelStartCutScenePlayer();
	if (cutScenePlayer1)
	{
		cutScenePlayer1->OnCutSceneEnd.BindUObject(this, &ACossacksGameMode::OnLevelStartCutSceneEnd);
		cutScenePlayer1->StartPlaying();
	}
	else 
	{
		OnLevelStartCutSceneEnd();
	}
}

void ACossacksGameMode::OnLevelStartCutSceneEnd()
{
	UGameplayStatics::PlaySound2D(GetWorld(), BattleStartSFX);

	if (AMusicManager::GetInstance())
		AMusicManager::GetInstance()->StartPlayingMusic();
}

void ACossacksGameMode::OnLevelFinishCutSceneEnd()
{
	GoToNextLevel();
}

void ACossacksGameMode::OnWin()
{
	if (AMusicManager::GetInstance())
		AMusicManager::GetInstance()->StopPlayingMusic();

	if (BattleFinishWidget)
		return;

	BattleFinishWidget = CreateWidget(GetWorld(), WinWidgetClass.Get());

	if (BattleFinishWidget)
		BattleFinishWidget->AddToViewport();
}

void ACossacksGameMode::OnLost()
{
	if (AMusicManager::GetInstance())
		AMusicManager::GetInstance()->StopPlayingMusic();

	if (BattleFinishWidget)
		return;

	BattleFinishWidget = CreateWidget(GetWorld(), LostWidgetClass.Get());

	if (BattleFinishWidget)
		BattleFinishWidget->AddToViewport();
}

void ACossacksGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTime += DeltaTime * GameMinutesPerRealSecond;
}

float ACossacksGameMode::GetGameTimeSeconds() const
{
	return GameTime * 60.0f;
}

float ACossacksGameMode::GetGameTimeMinutes() const
{
	return GameTime;
}

float ACossacksGameMode::GetGameTimeHours() const
{
	return GameTime / 60.0f;
}

void ACossacksGameMode::GoToNextLevel()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), NextLevel);
}
