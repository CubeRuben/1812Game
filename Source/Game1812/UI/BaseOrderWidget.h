#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseOrderWidget.generated.h"

UCLASS()
class GAME1812_API UBaseOrderWidget : public UUserWidget
{
	GENERATED_BODY()
	
public: 

	UFUNCTION(BlueprintNativeEvent)
	void Init(class UPieceOrderWidgetComponent* PieceOrderComponent);

	
};
