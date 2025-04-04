#include "PlayerInteractionComponent.h"

#include "../PlayerPawn.h"
#include "PlayerMovementComponent.h"
#include "Interactable.h"
#include "InteractableSortable.h"


UPlayerInteractionComponent::UPlayerInteractionComponent() :
	AllowedToInteract(true), AllowedToDrag(true), AllowedToSelect(true)
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentDraggable = nullptr;
	CurrentHovered = nullptr;

	InteractionDistance = 500.0f;
	DraggingHeight = 150.0f;
	AltDraggingHeight = 50.0f;
	RotateSpeed = 100.0f;
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

	const float MovementInterpolationSpeed = 20.0f;

	const bool isNotLookingAtMap = PlayerPawn->GetMovementComponent()->GetMapState() != EPlayerCameraState::LookingAtMap;
	const bool isNotAllowedToInteract = !AllowedToInteract;

	if (isNotLookingAtMap || isNotAllowedToInteract)
	{
		SetCurrentDraggable(nullptr);
		SetCurrentSelected(nullptr);
		SetCurrentHovered(nullptr);

		return;
	}

	FPlayerInput& playerInput = PlayerPawn->GetPlayerInput();

	AActor* interactableActor = nullptr;
	IInteractable* interactable = nullptr;

	FindInteractableAtCursor(interactableActor, interactable);

	if ((interactable != nullptr) && (playerInput.MouseLeftClick || playerInput.MouseRightClick))
	{
		if (!CurrentDraggable) 
		{
			if (playerInput.LeftShift) 
			{
				playerInput.MouseLeftClick = false;

				if (interactable->CanBeGrouped())
				{
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
			}
			else if (!InteractableActorsSelectedGroup.Contains(interactableActor))
			{
				ClearSelectedGroup();
			}
		}
	}

	if ((interactable == nullptr) && playerInput.MouseLeftClick)
	{
		playerInput.MouseLeftClick = false;

		bIsMultipleSelection = true;
		OnMultipleSelectionStart.Broadcast();

	}

	if (bIsMultipleSelection && !playerInput.MouseLeftHold)
	{
		bIsMultipleSelection = false;
		OnMultipleSelectionEnd.Broadcast();
	}

	if (CurrentDraggable)
	{
		if (playerInput.MouseLeftHold && !PlayerPawn->GetMovementComponent()->IsInGlobalMap()) 
		{
			HandleFormationInput();

			FHitResult hit = SingleCursorTrace(true);
			AActor* draggableActor = Cast<AActor>(CurrentDraggable);

			if (!draggableActor)
				return;

			FVector horizontalTargetLocation = hit.Location;
			horizontalTargetLocation.Z = draggableActor->GetActorLocation().Z;
			const FVector newHorizontalLocation = FMath::VInterpTo(draggableActor->GetActorLocation(), horizontalTargetLocation, DeltaTime, MovementInterpolationSpeed);
			const FVector deltaLocation = newHorizontalLocation - draggableActor->GetActorLocation();
			
			draggableActor->SetActorLocation(newHorizontalLocation);

			for (AActor* el : InteractableActorsSelectedGroup)
			{
				if (el == draggableActor)
					continue;

				el->AddActorWorldOffset(deltaLocation);
			}

			const bool altDragging = playerInput.MouseRightHold;
			const float draggingHeight = altDragging ? (AltDraggingHeight) : (DraggingHeight);

			{
				const FVector hoverLocation = GetSurfaceUnderActor(draggableActor) + FVector(0.0f, 0.0f, draggingHeight);
				const FVector newVerticalLocation = FMath::VInterpTo(draggableActor->GetActorLocation(), hoverLocation, DeltaTime, MovementInterpolationSpeed);

				draggableActor->SetActorLocation(newVerticalLocation);
			}

			for (AActor* el : InteractableActorsSelectedGroup)
			{
				if (el == draggableActor)
					continue;

				const FVector hoverLocation = GetSurfaceUnderActor(el) + FVector(0.0f, 0.0f, draggingHeight);
				const FVector newVerticalLocation = FMath::VInterpTo(el->GetActorLocation(), hoverLocation, DeltaTime, MovementInterpolationSpeed);

				el->SetActorLocation(newVerticalLocation);
			}

			const float rotationDirection = (-(float)playerInput.RotateLeft + (float)playerInput.RotateRight);
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
		if (playerInput.MouseLeftClick) 
		{
			playerInput.MouseLeftClick = false;

			if (interactable)
				SetCurrentDraggable(interactable);
		}
		else if (playerInput.MouseRightClick)
		{
			playerInput.MouseRightClick = false;

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

void UPlayerInteractionComponent::HandleFormationInput()
{
	FPlayerInput& playerInput = PlayerPawn->GetPlayerInput();
	
	if (!playerInput.MakeFormation.Contains(true))
		return;

	if (InteractableActorsSelectedGroup.Num() == 0) 
	{
		for (auto& input : playerInput.MakeFormation)
		{
			input = false;
		}
		return;
	}

	TArray<TScriptInterface<IInteractableSortable>> interactables(InteractableActorsSelectedGroup);

	for (auto& interactable : interactables) 
	{
		if (!interactable.GetInterface()) 
		{
			for (auto& input : playerInput.MakeFormation)
			{
				input = false;
			}
			return;
		}
	}

	interactables.Sort([](const TScriptInterface<IInteractableSortable>& el1, const TScriptInterface<IInteractableSortable>& el2) 
	{ 
		return el1.GetInterface()->GetPriority() > el2.GetInterface()->GetPriority(); 
	});

	for (int i = 0; i < playerInput.MakeFormation.Num(); i++) 
	{
		if (!playerInput.MakeFormation[i])
			continue;

		playerInput.MakeFormation[i] = false;

		TArray<FVector> offsets;
		if (!GetFormationOffsetArray(i, interactables.Num(), offsets))
			continue;

		AActor* mainActor = Cast<AActor>(CurrentDraggable);
		const FVector mainLocation = mainActor->GetActorLocation();
		const FRotator mainRotation = mainActor->GetActorRotation();

		for (int a = 0; a < interactables.Num(); a++)
		{
			AActor* actor = Cast<AActor>(interactables[a].GetObject());

			if (!actor)
				continue;
			
			const FVector newLocation = mainLocation + offsets[a].RotateAngleAxis(mainRotation.Yaw, FVector::UpVector);
			actor->SetActorLocation(newLocation);
			actor->SetActorRotation(mainRotation);
		}

		SetCurrentDraggable(Cast<IInteractable>(interactables[0].GetObject()));

	}
}

bool UPlayerInteractionComponent::GetFormationOffsetArray(int FormationType, int Amount, TArray<FVector>& OutArray)
{
	const float OFFSET = 45.0f;

	OutArray.SetNum(Amount);

	if (Amount <= 0)
		return false;

	OutArray[0] = FVector::ZeroVector;

	switch (FormationType)
	{
	case 0:
		{
			for (int i = 0; i < Amount - 1; i++)
			{
				const int mult = ((float)i / 2.0f + 1.0f);

				if (i % 2 == 0)
				{
					OutArray[i + 1] = FVector(0.0f, OFFSET * mult, 0.0f);
				}
				else
				{
					OutArray[i + 1] = FVector(0.0f, -OFFSET * mult, 0.0f);
				}
			}
		}
		return true;
	case 1:
		{
			const int firstRow = FMath::RoundToInt((float)Amount / 2.0f) - (Amount > 4);

			for (int i = 0; i < firstRow; i++) 
			{
				const int mult = ((float)i / 2.0f + 1.0f);

				if (i % 2 == 0) 
				{
					OutArray[i + 1] = FVector(0.0f, OFFSET * mult, 0.0f);
				}
				else 
				{
					OutArray[i + 1] = FVector(0.0f, -OFFSET * mult, 0.0f);
				}
			}

			const float secondRowOffset = ((Amount == 4) ? 0.0f : ((((2 * (firstRow + 1) - Amount) % 2) == 0) ? 0.0f : (((((Amount - 5) / 2) % 2) ? -1 : 1) * (OFFSET / 2.0f))));

			if (Amount > 3)
			{
				OutArray[firstRow + 1] = FVector(-OFFSET, 0.0f - secondRowOffset, 0.0f);
			}

			for (int i = 0; i < Amount - firstRow - 2; i++)
			{
				const int mult = ((float)i / 2.0f + 1.0f);

				if (i % 2 == 0)
				{
					OutArray[i + firstRow + 2] = FVector(-OFFSET, OFFSET * mult - secondRowOffset, 0.0f);
				}
				else
				{
					OutArray[i + firstRow + 2] = FVector(-OFFSET, -OFFSET * mult - secondRowOffset, 0.0f);
				}
			}
		}
		return true;
	default:
		return false;
	}
}

void UPlayerInteractionComponent::SetAllowedToInteract(bool NewAllowedToInteract)
{
	AllowedToInteract = NewAllowedToInteract;

	if (!AllowedToInteract)
		ClearSelectedGroup();
}

void UPlayerInteractionComponent::SetAllowedToDrag(bool NewAllowedToDrag)
{
	AllowedToDrag = NewAllowedToDrag;

	if (!AllowedToDrag && CurrentDraggable != nullptr)
		SetCurrentDraggable(nullptr);
}

void UPlayerInteractionComponent::SetAllowedToSelect(bool NewAllowedToSelect)
{
	AllowedToSelect = NewAllowedToSelect;

	if (!AllowedToSelect && CurrentSelected != nullptr)
		SetCurrentSelected(nullptr);
}

void UPlayerInteractionComponent::AddInteractableClassToWhitelist(TSubclassOf<UObject> Class)
{
	InteractableClassesWhitelist.Add(Class);
}

void UPlayerInteractionComponent::RemoveInteractableClassFromWhitelist(TSubclassOf<UObject> Class)
{
	InteractableClassesWhitelist.Remove(Class);
}

bool UPlayerInteractionComponent::IsInteractableClassAllowed(IInteractable* Object)
{
	return IsInteractableClassAllowed(Cast<UObject>(Object));
}

bool UPlayerInteractionComponent::IsInteractableClassAllowed(UObject* Object)
{
	if (!Object)
		return false;

	if (InteractableClassesWhitelist.IsEmpty())
		return true;

	for (TSubclassOf<UObject>& classPtr : InteractableClassesWhitelist)
	{
		if (!classPtr.Get())
			continue;

		if (Object->IsA(classPtr))
			return true;
	}

	return false;
}

bool UPlayerInteractionComponent::IsInteractableClassAllowed(TScriptInterface<class IInteractable> Interactable)
{
	return IsInteractableClassAllowed(Interactable.GetObject());
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
	if (CurrentSelected)
		SetCurrentSelected(nullptr);

	if (!PlayerPawn->GetPlayerInput().LeftShift)
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

void UPlayerInteractionComponent::RemoveInteractable(IInteractable* Interactable)
{
	if (!Interactable)
		return;

	if (CurrentDraggable == Interactable)
		SetCurrentDraggable(nullptr);

	if (CurrentHovered == Interactable)
		SetCurrentHovered(nullptr);

	if (CurrentSelected == Interactable)
		SetCurrentSelected(nullptr);

	if (InteractableActorsSelectedGroup.Remove(Cast<AActor>(Interactable)) == 0)
		return;

	Interactable->StopGroupSelectionHover();

	if (CurrentDraggable)
		Interactable->StopDragging();
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

	if (!AllowedToDrag || !IsInteractableClassAllowed(NewDraggable))
	{
		CurrentDraggable = nullptr;
		return;
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

	if (!IsInteractableClassAllowed(NewHovered))
	{
		CurrentHovered = nullptr;
		return;
	}

	if (NewHovered)
		NewHovered->StartCursorHover();

	CurrentHovered = NewHovered;
}

void UPlayerInteractionComponent::SetCurrentSelected(IInteractable* NewSelected)
{
	if (CurrentSelected)
		CurrentSelected->SelectionRemoved();

	if (!AllowedToSelect || !IsInteractableClassAllowed(NewSelected)) 
	{
		CurrentSelected = nullptr;
		return;
	}

	if (NewSelected)
		NewSelected->Selected();

	CurrentSelected = NewSelected;
}

FHitResult UPlayerInteractionComponent::SingleCursorTrace(bool IgnoreGroup)
{
	FVector cursorLocation, cursorDirection;
	PlayerPawn->GetLocalViewingPlayerController()->DeprojectMousePositionToWorld(cursorLocation, cursorDirection);

	FHitResult hit;
	FCollisionQueryParams collisionParams;

	collisionParams.bTraceComplex = true;

	AActor* draggableActor = Cast<AActor>(CurrentDraggable);

	if (draggableActor)
		collisionParams.AddIgnoredActor(draggableActor);

	if (IgnoreGroup)
		collisionParams.AddIgnoredActors(InteractableActorsSelectedGroup);
	
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

	if (IsInteractableClassAllowed(hit.GetActor())) 
	{
		Actor = hit.GetActor();
		Interactable = Cast<IInteractable>(Actor);
	}
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
