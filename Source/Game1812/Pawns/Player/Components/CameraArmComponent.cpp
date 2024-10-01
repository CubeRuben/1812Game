#include "CameraArmComponent.h"

#include "PlayerMovementComponent.h"
#include "../PlayerPawn.h"

UCameraArmComponent::UCameraArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxLength = 400.0f;
	MinLength = 20.0f;
	Interpolation = 5.0f;
}

void UCameraArmComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentLength = MaxLength;
	TargetLength = CurrentLength;

	if (!GetOwner()) return;

	PlayerPawn = Cast<APlayerPawn>(GetOwner());

	if (!PlayerPawn) return;
}


float UCameraArmComponent::AddTargetLength(float deltaLength)
{
	float savedLength = TargetLength;

	float minLength = PlayerPawn->GetMovementComponent()->IsInGlobalMapBounds() ? MinLength : PlayerPawn->GetMovementComponent()->GetGlobalMapArmLength();

	TargetLength = FMath::Clamp(TargetLength + deltaLength, minLength, MaxLength);
	return savedLength - TargetLength;
}

void UCameraArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentLength = FMath::FInterpTo(CurrentLength, TargetLength, DeltaTime, Interpolation);

	PlayerPawn->GetCameraArmPoint()->SetRelativeLocation(FVector(-CurrentLength, 0, 0));
}

