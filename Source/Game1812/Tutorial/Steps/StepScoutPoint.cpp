#include "StepScoutPoint.h"

#include "../TutorialManager.h"
#include "../TutorialScoutDiscoverPoint.h"

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
	for (TWeakObjectPtr<ATutorialScoutDiscoverPoint> point : PointsToDiscover)
	{
		if (!point.IsValid())
			continue;

		point->SetPointActive(false);
	}
}
