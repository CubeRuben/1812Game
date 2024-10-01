#include "PlayerCameraSpot.h"

#include <Camera/CameraComponent.h>

APlayerCameraSpot::APlayerCameraSpot()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	RootComponent = CameraComponent;
}

