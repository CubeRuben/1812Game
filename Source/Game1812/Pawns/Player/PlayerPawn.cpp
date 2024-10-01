#include "PlayerPawn.h"

#include <Camera/CameraComponent.h>
#include <Components/ArrowComponent.h>

#include "Components/PlayerMovementComponent.h"
#include "Components/PlayerInteractionComponent.h"
#include "Components/CameraArmComponent.h"
#include "../../Configs/InputMappingConfig.h"

#include <InputActionValue.h>
#include <InputMappingContext.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

FPlayerInput::FPlayerInput()
{
	MouseScroll = 0.0f;

	MoveForward = false;
	MoveBack = false;
	MoveLeft = false;
	MoveRight = false;
	LookAtMap = false;
	MouseLeftClick = false;
	MouseLeftHold = false;
	MouseRightHold = false;
	RotateLeft = false;
	RotateRight = false;
}

APlayerPawn* APlayerPawn::Instance = nullptr;

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CameraArmComponent = CreateDefaultSubobject<UCameraArmComponent>(TEXT("Camera Arm"));
	CameraArmComponent->SetupAttachment(RootComponent);

	CameraArmPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Camera Point"));
	CameraArmPoint->SetupAttachment(CameraArmComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraArmPoint);

	MovementComponent = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("Movement Component"));
	InteractionComponent = CreateDefaultSubobject<UPlayerInteractionComponent>(TEXT("Interaction Component"));
}


void APlayerPawn::MouseScroll(const FInputActionValue& Value) { PlayerInput.MouseScroll = Value.Get<float>(); };
void APlayerPawn::MouseLeftClick(const FInputActionValue& Value) { PlayerInput.MouseLeftClick = PlayerInput.MouseLeftHold = Value.Get<bool>(); };
void APlayerPawn::MouseRightClick(const FInputActionValue& Value) { PlayerInput.MouseRightClick = PlayerInput.MouseRightHold = Value.Get<bool>(); };
void APlayerPawn::MoveForward(const FInputActionValue& Value) { PlayerInput.MoveForward = Value.Get<bool>(); };
void APlayerPawn::MoveBack(const FInputActionValue& Value) { PlayerInput.MoveBack = Value.Get<bool>(); };
void APlayerPawn::MoveLeft(const FInputActionValue& Value) { PlayerInput.MoveLeft = Value.Get<bool>(); };
void APlayerPawn::MoveRight(const FInputActionValue& Value) { PlayerInput.MoveRight = Value.Get<bool>(); };
void APlayerPawn::LookAtMap(const FInputActionValue& Value) { PlayerInput.LookAtMap = Value.Get<bool>(); };
void APlayerPawn::LeftShift(const FInputActionValue& Value) { PlayerInput.LeftShift = Value.Get<bool>(); };
void APlayerPawn::RotateLeft(const FInputActionValue& Value) { PlayerInput.RotateLeft = Value.Get<bool>(); };
void APlayerPawn::RotateRight(const FInputActionValue& Value) { PlayerInput.RotateRight = Value.Get<bool>(); };

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;

	if (GetLocalViewingPlayerController()) GetLocalViewingPlayerController()->SetShowMouseCursor(true);
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!InputConfig) 
		return;

	APlayerController* playerController = Cast<APlayerController>(GetController());

	if (!playerController) 
		return;

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	if (!subsystem) 
		return;

	subsystem->ClearAllMappings();
	subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	PlayerEnhancedInputComponent->BindAction(InputConfig->MouseScroll, ETriggerEvent::Triggered, this, &APlayerPawn::MouseScroll);
	PlayerEnhancedInputComponent->BindAction(InputConfig->MouseLeftClick, ETriggerEvent::Triggered, this, &APlayerPawn::MouseLeftClick);
	PlayerEnhancedInputComponent->BindAction(InputConfig->MouseRightClick, ETriggerEvent::Triggered, this, &APlayerPawn::MouseRightClick);

	PlayerEnhancedInputComponent->BindAction(InputConfig->MoveForward, ETriggerEvent::Triggered, this, &APlayerPawn::MoveForward);
	PlayerEnhancedInputComponent->BindAction(InputConfig->MoveBack, ETriggerEvent::Triggered, this, &APlayerPawn::MoveBack);
	PlayerEnhancedInputComponent->BindAction(InputConfig->MoveLeft, ETriggerEvent::Triggered, this, &APlayerPawn::MoveLeft);
	PlayerEnhancedInputComponent->BindAction(InputConfig->MoveRight, ETriggerEvent::Triggered, this, &APlayerPawn::MoveRight);

	PlayerEnhancedInputComponent->BindAction(InputConfig->LookAtMap, ETriggerEvent::Triggered, this, &APlayerPawn::LookAtMap);

	PlayerEnhancedInputComponent->BindAction(InputConfig->LeftShift, ETriggerEvent::Triggered, this, &APlayerPawn::LeftShift);

	PlayerEnhancedInputComponent->BindAction(InputConfig->RotateLeft, ETriggerEvent::Triggered, this, &APlayerPawn::RotateLeft);
	PlayerEnhancedInputComponent->BindAction(InputConfig->RotateRight, ETriggerEvent::Triggered, this, &APlayerPawn::RotateRight);
}

