#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MusicManager.generated.h"

UENUM()
enum class EMusicState : uint8 
{
	Neutral		= 0,
	InBattle	= 1
};

UCLASS()
class GAME1812_API AMusicManager : public AActor
{
	GENERATED_BODY()
	
public:	

	AMusicManager();

protected:

	static TWeakObjectPtr<AMusicManager> Instance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundCue* Music;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TransitionSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TransitionTimeoutToNeutral;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimerToNeutral;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMusicState CurrentState;

	class USoundNodeMixer* SoundMixerNode;
	
	void BeginPlay() override;
	void BeginDestroy() override;

	void FindMixer();
	void ResetMixer();

	void OnBattleBegin(bool bIsInFog);

public:	

	static AMusicManager* GetInstance() { return Instance.Get(); }

	virtual void Tick(float DeltaTime) override;

	void SetState(EMusicState NewCurrentState);
	
	void StartPlayingMusic();
	void StopPlayingMusic();

};
