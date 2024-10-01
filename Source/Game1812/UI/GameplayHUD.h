#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameplayHUD.generated.h"

UCLASS()
class GAME1812_API AGameplayHUD : public AHUD
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category=HUD)
	void GetInteractableActorsInSelectionRectangleByCenter(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<TScriptInterface<class IInteractable>>& OutActors);

};
