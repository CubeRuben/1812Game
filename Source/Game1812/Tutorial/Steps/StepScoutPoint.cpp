#include "StepScoutPoint.h"

#include "../TutorialManager.h"
#include "../TutorialScoutDiscoverPoint.h"
#include "../../Actors/Pieces/ScoutPiece.h"
#include "../../Pawns/Player/PlayerPawn.h"
#include "../../Pawns/Player/Components/PlayerInteractionComponent.h"

UStepScoutPoint::UStepScoutPoint() :
	PointsCounter(0)
{
}

void UStepScoutPoint::OnPointDiscover()
{
	PointsCounter--;

	if (PointsCounter <= 0) 
	{
		Manager->NextStep();
	}
}

void UStepScoutPoint::StepStart()
{
	Manager->GetPlayerPawn()->GetInteractionComponent()->AddInteractableClassToWhitelist(AScoutPiece::StaticClass());

	for (TWeakObjectPtr<ATutorialScoutDiscoverPoint> point : PointsToDiscover) 
	{
		if (!point.IsValid())
			continue;

		PointsCounter++;

		point->BindOnPointDiscover(FOnPointDiscoverDelegate::FDelegate::CreateUObject(this, &UStepScoutPoint::OnPointDiscover));
		point->SetPointActive(true);
	}
}

void UStepScoutPoint::StepEnd()
{
	Manager->GetPlayerPawn()->GetInteractionComponent()->RemoveInteractableClassFromWhitelist(AScoutPiece::StaticClass());

	for (TWeakObjectPtr<ATutorialScoutDiscoverPoint> point : PointsToDiscover)
	{
		if (!point.IsValid())
			continue;

		point->SetPointActive(false);
	}
}
