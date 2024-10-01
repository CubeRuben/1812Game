#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CossacksGameMode.generated.h"

UCLASS()
class GAME1812_API ACossacksGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ACossacksGameMode();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GameTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GameMinutesPerRealSecond;

public:

	void Tick(float DeltaTime) override;

	float GetGameTimeMinutes() const;
};
