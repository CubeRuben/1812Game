#include "CossacksGameMode.h"

ACossacksGameMode::ACossacksGameMode()
{
	GameTime = 0.f;
	GameMinutesPerRealSecond = 5.f / 60.f;
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
