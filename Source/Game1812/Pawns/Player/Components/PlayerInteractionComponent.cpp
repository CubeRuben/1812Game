#include "PlayerInteractionComponent.h"

#include "../PlayerPawn.h"
#include "PlayerMovementComponent.h"
#include "Interactable.h"


UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentDraggable = nullptr;
	CurrentHovered = nullptr;

	InteractionDistance = 500.f;
	DraggingHeight = 150.f;
	AltDraggingHeight = 50.f;
	RotateSpeed = 100.f;
}

void UPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner())
		return DestroyComponent();

	PlayerPawn = Cast<APlayerPawn>(GetOwner());

	if (!PlayerPawn)
		return DestroyComponent();
}

void UPlayerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#define MOVEMENT_INTERPOLATION_SPEED 20.0f

	if (PlayerPawn->GetMovementComponent()->GetMapState() != EPlayerCameraState::LookingAtMap) 
	{
		SetCurrentDraggable(nullptr);
		SetCurrentSelected(nullptr);
		SetCurrentHovered(nullptr);

		return;
	}

	AActor* interactableActor = nullptr;
	IInteractable* interactable = nullptr;

	FindInteractableAtCursor(interactableActor, interactable);

	if (PlayerPawn->GetPlayerInput()->MouseLeftClick || PlayerPawn->GetPlayerInput()->MouseRightClick)
	{
		if (!CurrentDraggable) 
		{
			if (PlayerPawn->GetPlayerInput()->LeftShift) 
			{
				PlayerPawn->GetPlayerInput()->MouseLeftClick = false;

				if (InteractableActorsSelectedGroup.Contains(interactableActor))
				{
					InteractableActorsSelectedGroup.Remove(interactableActor);
					interactable->StopGroupSelectionHover();
				}
				else 
				{
					InteractableActorsSelectedGroup.Add(interactableActor);
					interactable->StartGroupSelectionHover();
				}
			}
			else if (!InteractableActorsSelectedGroup.Contains(interactableActor))
			{
				ClearSelectedGroup();
			}
		}
	}

	if ((interactable == nullptr) && PlayerPawn->GetPlayerInput()->MouseLeftClick)
	{
		PlayerPawn->GetPlayerInput()->MouseLeftClick = false;

		bIsMultipleSelection = true;
		OnMultipleSelectionStart.Broadcast();

	}

	if (bIsMultipleSelection && !PlayerPawn->GetPlayerInput()->MouseLeftHold)
	{
		bIsMultipleSelection = false;
		OnMultipleSelectionEnd.Broadcast();
	}

	if (CurrentDraggable)
	{
		if (PlayerPawn->GetPlayerInput()->MouseLeftHold && !PlayerPawn->GetMovementComponent()->IsInGlobalMap()) 
		{
			FHitResult hit = SingleCursorTrace();
			AActor* draggableActor = Cast<AActor>(CurrentDraggable);

			if (!draggableActor)
				return;

			FVector horizontalTargetLocation = hit.Location;
			horizontalTargetLocation.Z = draggableActor->GetActorLocation().Z;
			const FVector newHorizontalLocation = FMath::VInterpTo(draggableActor->GetActorLocation(), horizontalTargetLocation, DeltaTime, MOVEMENT_INTERPOLATION_SPEED);
			const FVector deltaLocation = newHorizontalLocation - draggableActor->GetActorLocation();
			
			draggableActor->SetActorLocation(newHorizontalLocation);

			for (AActor* el : InteractableActorsSelectedGroup)
			{
				if (el == draggableActor)
					continue;

				el->AddActorWorldOffset(deltaLocation);
			}

			const bool altDragging = PlayerPawn->GetPlayerInput()->MouseRightHold;
			const float draggingHeight = altDragging ? (AltDraggingHeight) : (DraggingHeight);

			{
				const FVector hoverLocation = GetSurfaceUnderActor(draggableActor) + FVector(0.0f, 0.0f, draggingHeight);
				const FVector newVerticalLocation = FMath::VInterpTo(draggableActor->GetActorLocation(), hoverLocation, DeltaTime, MOVEMENT_INTERPOLATION_SPEED);

				draggableActor->SetActorLocation(newVerticalLocation);
			}

			for (AActor* el : InteractableActorsSelectedGroup)
			{
				if (el == draggableActor)
					continue;

				const FVector hoverLocation = GetSurfaceUnderActor(el) + FVector(0.0f, 0.0f, draggingHeight);
				const FVector newVerticalLocation = FMath::VInterpTo(el->GetActorLocation(), hoverLocation, DeltaTime, MOVEMENT_INTERPOLATION_SPEED);

				el->SetActorLocation(newVerticalLocation);
			}

			const float rotationDirection = (-(float)PlayerPawn->GetPlayerInput()->RotateLeft + (float)PlayerPawn->GetPlayerInput()->RotateRight);
			const float rotationDelta = rotationDirection * RotateSpeed * DeltaTime;

			draggableActor->AddActorWorldRotation(FRotator(0.0f, rotationDelta, 0.0f));

			for (AActor* el : InteractableActorsSelectedGroup)
			{
				if (el == draggableActor)
					continue;

				const FVector relativeLocation = el->GetActorLocation() - draggableActor->GetActorLocation();
				const FVector rotatedRelativeLocation = relativeLocation.RotateAngleAxis(rotationDelta, FVector::UpVector);

				el->SetActorLocation(draggableActor->GetActorLocation() + rotatedRelativeLocation);
				el->AddActorWorldRotation(FRotator(0.0f, rotationDelta, 0.0f));
			}
		}
		else 
		{
			SetCurrentDraggable(nullptr);
		}
	}
	else
	{
		if (PlayerPawn->GetPlayerInput()->MouseLeftClick) 
		{
			PlayerPawn->GetPlayerInput()->MouseLeftClick = false;

			if (interactable)
				SetCurrentDraggable(interactable);
		}
		else if (PlayerPawn->GetPlayerInput()->MouseRightClick)
		{
			PlayerPawn->GetPlayerInput()->MouseRightClick = false;

			if (CurrentSelected == interactable)
			{
				SetCurrentSelected(nullptr);
			}
			else 
			{
				SetCurrentSelected(interactable);
			}
		}
	}

	if (interactable != CurrentHovered)
		SetCurrentHovered(interactable);
}

void UPlayerInteractionComponent::ClearSelectedGroup()
{
	for (AActor* el : InteractableActorsSelectedGroup)
	{
		IInteractable* interactable = Cast<IInteractable>(el);

		if (!interactable)
			continue;

		interactable->StopGroupSelectionHover();
	}

	InteractableActorsSelectedGroup.Reset();
}

void UPlayerInteractionComponent::SetSelectedGroup(const TArray<TScriptInterface<class IInteractable>>& NewGroup)
{
	ClearSelectedGroup();

	for (const TScriptInterface<IInteractable>& el : NewGroup)
	{
		if (!el.GetInterface())
			continue;

		el.GetInterface()->StartGroupSelectionHover();

		AActor* actor = Cast<AActor>(el.GetObject());

		if (!actor)
			continue;

		InteractableActorsSelectedGroup.Add(actor);
	}
}

void UPlayerInteractionComponent::SetCurrentDraggable(IInteractable* NewDraggable)
{
	if (CurrentDraggable) 
	{
		CurrentDraggable->StopDragging();

		for (AActor* el : InteractableActorsSelectedGroup)
		{
			IInteractable* interactable = Cast<IInteractable>(el);

			if (interactable)
				interactable->StopDragging();
		}
	}

	if (NewDraggable) 
	{
		NewDraggable->StartDragging();

		for (AActor* el : InteractableActorsSelectedGroup)
		{
			IInteractable* interactable = Cast<IInteractable>(el);

			if (interactable)
				interactable->StartDragging();
		}
	}

	CurrentDraggable = NewDraggable;
}

void UPlayerInteractionComponent::SetCurrentHovered(IInteractable* NewHovered)
{
	if (CurrentHovered)
		CurrentHovered->StopCursorHover();

	if (NewHovered)
		NewHovered->StartCursorHover();

	CurrentHovered = NewHovered;
}

void UPlayerInteractionComponent::SetCurrentSelected(IInteractable* NewSelected)
{
	if (CurrentSelected)
		CurrentSelected->SelectionRemoved();

	if (NewSelected)
		NewSelected->Selected();

	CurrentSelected = NewSelected;
}

FHitResult UPlayerInteractionComponent::SingleCursorTrace()
{
	FVector cursorLocation, cursorDirection;
	PlayerPawn->GetLocalViewingPlayerController()->DeprojectMousePositionToWorld(cursorLocation, cursorDirection);

	FHitResult hit;
	FCollisionQueryParams collisionParams;

	collisionParams.bTraceComplex = true;

	AActor* draggableActor = Cast<AActor>(CurrentDraggable);

	if (draggableActor)
		collisionParams.AddIgnoredActor(draggableActor);
	
	GetWorld()->LineTraceSingleByChannel(hit, cursorLocation, cursorLocation + cursorDirection * InteractionDistance, ECC_Visibility, collisionParams);

	return hit;
}

void UPlayerInteractionComponent::FindInteractableAtCursor(AActor*& Actor, class IInteractable*& Interactable)
{
	FHitResult hit = SingleCursorTrace();

	if (!hit.bBlockingHit) 
		return;

	if (!hit.GetActor()) 
		return;

	Actor = hit.GetActor();
	Interactable = Cast<IInteractable>(Actor);
}

FVector UPlayerInteractionComponent::GetSurfaceUnderActor(AActor* Actor)
{
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(Actor);

	FHitResult hit;

	const FVector location = Actor->GetActorLocation();

	if (GetWorld()->LineTraceSingleByChannel(hit, location + FVector(0.0f, 0.0f, 50.0f), location - FVector(0.0f, 0.0f, 2000.0f), ECC_Visibility, collisionParams))
		return hit.ImpactPoint;

	return location;
}
