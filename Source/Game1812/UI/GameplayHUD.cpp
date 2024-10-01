#include "GameplayHUD.h"

#include "../Pawns/Player/Components/Interactable.h"

#include <EngineUtils.h>

void AGameplayHUD::GetInteractableActorsInSelectionRectangleByCenter(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<TScriptInterface<IInteractable>>& OutActors)
{
	OutActors.Reset();

	FBox2D selectionRectangle(ForceInit);

	selectionRectangle += FirstPoint;
	selectionRectangle += SecondPoint;

	for (TActorIterator<AActor> iterator(GetWorld()); iterator; ++iterator)
	{
		AActor* actor = *iterator;

		TScriptInterface<IInteractable> interactable(actor);

		if (!interactable.GetInterface())
			continue;

		const FVector center = actor->GetActorLocation();

		FBox2D actorBox2D(ForceInit);

		const FVector projectedWorldLocation = Project(center, true);
		actorBox2D += FVector2D(projectedWorldLocation.X, projectedWorldLocation.Y);

		if (actorBox2D.bIsValid)
		{
			if (selectionRectangle.IsInside(actorBox2D))
			{
				OutActors.Add(actor);
			}
		}
	}
}
