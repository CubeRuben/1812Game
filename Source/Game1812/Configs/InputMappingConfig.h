#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputMappingConfig.generated.h"

UCLASS()
class GAME1812_API UInputMappingConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* MouseScroll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* MouseLeftClick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* MouseRightClick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* MoveForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* MoveLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* MoveBack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* MoveRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* LookAtMap;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* LeftShift;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* RotateLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* RotateRight;
};