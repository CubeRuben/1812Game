#include "FormationMovement.h"

#include "../Units/CombatUnit.h"

UFormationMovement::UFormationMovement()
{

}

float UFormationMovement::FindMinimalMovementSpeed()
{
	float minSpeed = -100;
	
	for (auto& unit : CombatUnits) 
	{
		if (!unit)
			continue;

		const float speed = unit->GetMovementSpeed();

		if ((minSpeed < 0.0f) || (minSpeed > speed))
		{
			minSpeed = speed;
		}
	}

	return minSpeed;
}

float UFormationMovement::FindMinimalRotationSpeed()
{
	float minSpeed = -100;

	for (auto& unit : CombatUnits)
	{
		if (!unit)
			continue;

		const float speed = unit->GetRotationSpeed();

		if ((minSpeed < 0.0f) || (minSpeed > speed))
		{
			minSpeed = speed;
		}
	}

	return minSpeed;
}

float UFormationMovement::GetMovementSpeed()
{
	return FindMinimalMovementSpeed();
}

float UFormationMovement::GetRotationSpeed()
{
	return FindMinimalRotationSpeed();
}
