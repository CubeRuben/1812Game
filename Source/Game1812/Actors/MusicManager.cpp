#include "MusicManager.h"

#include "../Pawns/Unit/Components/UnitCombatComponent.h"

#include <Sound/SoundCue.h>
#include <Sound/SoundNodeMixer.h>
#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>

TWeakObjectPtr<AMusicManager> AMusicManager::Instance = nullptr;

AMusicManager::AMusicManager() :
	AudioComponent(nullptr),
	Music(nullptr),
	TransitionSpeed(0.5f),
	TransitionTimeoutToNeutral(10.0f),
	TimerToNeutral(0.0f),
	SoundMixerNode(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMusicManager::BeginPlay()
{
	Super::BeginPlay();

	if (!Instance.Get())
		Instance = this;

	UUnitCombatComponent::OnBattleBeginGlobal.AddUObject(this, &AMusicManager::OnBattleBegin);
}

void AMusicManager::BeginDestroy()
{
	Super::BeginDestroy();

	if (Instance == this)
		Instance = nullptr;
}

void AMusicManager::FindMixer()
{
	if (SoundMixerNode)
		return;

	/*if (!Music->FirstNode)
		return;*/

	SoundMixerNode = Cast<USoundNodeMixer>(Music->FirstNode);

	/*for (USoundNode* const & node : Music->FirstNode->ChildNodes) 
	{
		SoundMixerNode = Cast<USoundNodeMixer>(node);

		if (SoundMixerNode)
			break;
	}*/
}

void AMusicManager::ResetMixer()
{
	if (!SoundMixerNode)
		return;

	SoundMixerNode->InputVolume[0] = 1.0f;
	for (int i = 1; i < SoundMixerNode->InputVolume.Num(); i++)
	{
		SoundMixerNode->InputVolume[i] = 0.01;
	}
}

void AMusicManager::OnBattleBegin(bool bIsInFog)
{
	if (bIsInFog)
		return;

	SetState(EMusicState::InBattle);
}

void AMusicManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState != EMusicState::Neutral)
	{
		TimerToNeutral -= DeltaTime;

		if (TimerToNeutral <= 0.0f)
			CurrentState = EMusicState::Neutral;
	}

	if (!SoundMixerNode)
		return;

	const float minVolume = 0.01f;
	const float maxVolume = 1.0f;

	for (int i = 0; i < SoundMixerNode->InputVolume.Num(); i++)
	{
		const float delta = DeltaTime * TransitionSpeed;

		if ((uint8)CurrentState == i) 
		{
			SoundMixerNode->InputVolume[i] = FMath::Clamp(SoundMixerNode->InputVolume[i] + delta, minVolume, maxVolume);
		}
		else 
		{
			SoundMixerNode->InputVolume[i] = FMath::Clamp(SoundMixerNode->InputVolume[i] - delta, minVolume, maxVolume);
		}
	}
}

void AMusicManager::SetState(EMusicState NewCurrentState)
{
	if (NewCurrentState != EMusicState::Neutral)
	{
		TimerToNeutral = TransitionTimeoutToNeutral;
	}

	CurrentState = NewCurrentState;
}

void AMusicManager::StartPlayingMusic()
{
	if (!Music)
		return;

	FindMixer();
	ResetMixer();

	if (!AudioComponent)
		AudioComponent = UGameplayStatics::CreateSound2D(GetWorld(), Music);

	if (AudioComponent)
		AudioComponent->Play();
}

void AMusicManager::StopPlayingMusic()
{
	if (AudioComponent)
		AudioComponent->Stop();
}

