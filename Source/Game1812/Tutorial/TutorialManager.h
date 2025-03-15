#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialManager.generated.h"

UCLASS()
class GAME1812_API ATutorialManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ATutorialManager();

protected:

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TArray<class UTutorialBaseStep*> TutorialSteps;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentStepIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int DialogsIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TipsIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimerDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UTutorialWidget> TutorialWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTutorialWidget* TutorialWidget;

	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class APlayerPawn> PlayerPawn;

	virtual void BeginPlay() override;

	void NextDialog();
	void NextTip();

public:	

	virtual void Tick(float DeltaTime) override;

	class APlayerPawn* GetPlayerPawn();

	void BeginStep(int StepIndex);
	void NextStep();

	UFUNCTION(BlueprintCallable)
	void ForceFinish();

};
