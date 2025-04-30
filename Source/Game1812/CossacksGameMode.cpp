#include "CossacksGameMode.h"

#include "Actors/CutScenePlayer.h"
#include "ObjectivesSystem/BattleObjectivesManager.h"

#include <Kismet/GameplayStatics.h>
#include <Components/AudioComponent.h>

ACossacksGameMode::ACossacksGameMode() :
	GameTime(0.0f),
	GameMinutesPerRealSecond(5.0f / 60.0f),
	BattleStartSFX(nullptr),
	BattleMusic(nullptr),
	BattleMusicComponent(nullptr)
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
	BattleMusicComponent = UGameplayStatics::CreateSound2D(GetWorld(), BattleMusic);

	if (BattleMusicComponent)
		BattleMusicComponent->Play();
}

void ACossacksGameMode::OnLevelFinishCutSceneEnd()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), NextLevel);
}

void ACossacksGameMode::OnWin()
{
	if (BattleMusicComponent)
		BattleMusicComponent->Stop();

	ACutScenePlayer* cutScenePlayer2 = ACutScenePlayer::GetLevelFinishCutScenePlayer();

	if (cutScenePlayer2) 
	{
		cutScenePlayer2->StartPlaying();
	}
	else 
	{
		OnLevelFinishCutSceneEnd();
	}
}

void ACossacksGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTime += DeltaTime * GameMinutesPerRealSecond;
}

float ACossacksGameMode::GetGameTimeMinutes() const
{
	return GameTime;
}
