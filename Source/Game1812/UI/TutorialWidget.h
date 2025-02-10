#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

UCLASS()
class GAME1812_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent)
	void InitManager(class ATutorialManager* TutorialManager);

	UFUNCTION(BlueprintNativeEvent)
	void ChangeDialogText(const FText& Text);

	UFUNCTION(BlueprintNativeEvent)
	void ChangeTipText(const FText& Text);
};
