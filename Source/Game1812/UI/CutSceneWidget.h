#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CutSceneWidget.generated.h"

UCLASS()
class GAME1812_API UCutSceneWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void StartPlaying(class ACutScenePlayer* CutScenePlayer);
	
};
