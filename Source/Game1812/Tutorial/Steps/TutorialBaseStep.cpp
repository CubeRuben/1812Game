#include "TutorialBaseStep.h"

UTutorialBaseStep::UTutorialBaseStep()
{

}

void UTutorialBaseStep::StepStart(ATutorialManager* TutorialManager)
{
	Manager = TutorialManager;

	StepStart();
}

void UTutorialBaseStep::StepStart()
{

}

void UTutorialBaseStep::StepEnd()
{

}
