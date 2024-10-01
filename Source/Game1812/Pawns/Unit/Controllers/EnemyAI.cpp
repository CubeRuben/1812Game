#include "EnemyAI.h"

#include "EnemyUnitController.h"
#include "../Units/CombatUnit.h"
#include "../Orders/UnitOrder.h"
#include "../../../Actors/HeadQuarters.h"
#include "../../../CossacksGameMode.h"

UCombatFormation::UCombatFormation()
{

}

void UCombatFormation::ValidateControllers()
{
	UnitControllers = UnitControllers.FilterByPredicate([](const TWeakObjectPtr<class AEnemyUnitController>& el) { return el.IsValid() && el->GetCombatUnit(); });
}

FVector UCombatFormation::GetCenterLocation(bool SkipValidation)
{
	if (!SkipValidation)
		ValidateControllers();

	if (UnitControllers.IsEmpty())
		return FVector::ZeroVector;

	FVector meanLocation = FVector::ZeroVector;

	for (TWeakObjectPtr<AEnemyUnitController> controller : UnitControllers)
	{
		meanLocation += controller->GetCombatUnit()->GetActorLocation();
	}

	meanLocation /= UnitControllers.Num();

	return meanLocation;
}

void UCombatFormation::MoveFormationTo(const FVector& Location, float YawRotation, bool SkipValidation)
{
	if (!SkipValidation)
		ValidateControllers();

	AEnemyAI* ai = AEnemyAI::GetInstance();

	for (int i = 0; i < UnitControllers.Num(); i++)
	{
		UCombatUnitOrder* unitOrder = UnitControllers[i]->GetCombatUnit()->GetCombatUnitOrder();

		if (!unitOrder)
			unitOrder = NewObject<UCombatUnitOrder>(this);

		FVector offset(ai->GetFormationUnitOffset(UnitControllers.Num(), i), 0.f);
		offset = offset.RotateAngleAxis(YawRotation, FVector::UpVector);

		unitOrder->Location = Location + FVector(0.f, 0.f, 50.f) + offset;
		unitOrder->YawRotation = YawRotation;

		UnitControllers[i]->GetCombatUnit()->AssignOrder(unitOrder);
	}
}

void UCombatFormation::AssembleFormation(float YawRotation)
{
	ValidateControllers();

	if (UnitControllers.IsEmpty())
		return;

	MoveFormationTo(GetCenterLocation(true), YawRotation, true);
}

AEnemyAI* AEnemyAI::Instance = nullptr;

AEnemyAI::AEnemyAI()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;

	MoveDistance = 100.f;
	MoveTimeout = 5.f;
	MoveRotationOffset = 25.f;
	NextTimeForMove = 0.f;

	CombatFormationsAmount = 3;
	AIState = EEnemyAIState::PreparingForBattle;

	FormationUnitOffsets = TArray<FFormationUnitOffset> {
		FFormationUnitOffset({ FVector2D(0.f, 0.f) }),
		FFormationUnitOffset({ FVector2D(0.f, -15.f), FVector2D(0.f, 15.f) }),
		FFormationUnitOffset({ FVector2D(0.f, -30.f), FVector2D(0.f, 0.f), FVector2D(0.f, 30.f) }),
		FFormationUnitOffset({ FVector2D(15.f, -15.f), FVector2D(15.f, 15.f), FVector2D(-15.f, -15.f), FVector2D(-15.f, 15.f) }),
		FFormationUnitOffset({ FVector2D(15.f, -30.f), FVector2D(15.f, 0.f), FVector2D(15.f, 30.f), FVector2D(-15.f, -15.f), FVector2D(-15.f, 15.f) }),
		FFormationUnitOffset({ FVector2D(15.f, -30.f), FVector2D(15.f, 0.f), FVector2D(15.f, 30.f), FVector2D(-15.f, -30.f), FVector2D(-15.f, 0.f), FVector2D(-15.f, 30.f) }),
		FFormationUnitOffset({ FVector2D(15.f, -45.f), FVector2D(15.f, -15.f), FVector2D(15.f, 15.f), FVector2D(15.f, 45.f), FVector2D(-15.f, -30.f), FVector2D(-15.f, 0.f), FVector2D(-15.f, 30.f) }),
		FFormationUnitOffset({ FVector2D(15.f, -45.f), FVector2D(15.f, -15.f), FVector2D(15.f, 15.f), FVector2D(15.f, 45.f),  FVector2D(-15.f, -45.f), FVector2D(-15.f, -15.f), FVector2D(-15.f, 15.f), FVector2D(-15.f, 45.f) }),
		FFormationUnitOffset({ FVector2D(30.f, -30.f), FVector2D(30.f, 0.f), FVector2D(30.f, 30.f), FVector2D(0.f, -30.f), FVector2D(0.f, 0.f), FVector2D(0.f, 30.f), FVector2D(-30.f, -30.f), FVector2D(-30.f, 0.f), FVector2D(-30.f, 30.f) }),
	};
}

void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	NextTimeForMove = MoveTimeout;

	Instance = this;

	CombatFormations.Empty();

	FTimerHandle handler;
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &AEnemyAI::CreateFormationsTimeout));

	GameMode = Cast<ACossacksGameMode>(GetWorld()->GetAuthGameMode());
}

void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AIState == EEnemyAIState::PreparingForBattle) 
	{
		if (CombatFormations.IsEmpty())
			return;

		for (UCombatFormation* formation : CombatFormations) 
		{
			const float rotation = GetRotationToHQ(formation->GetCenterLocation());
			formation->AssembleFormation(rotation);
		}

		AIState = EEnemyAIState::Attacking;
	}
	else if (AIState == EEnemyAIState::Attacking) 
	{
		if (!GameMode)
			return;

		if (GameMode->GetGameTimeMinutes() < NextTimeForMove)
			return;

		NextTimeForMove += MoveTimeout;

		for (UCombatFormation* formation : CombatFormations) 
		{
			const FVector center = formation->GetCenterLocation();
			float rotation = GetRotationToHQ(center);

			rotation += FMath::RandRange(-MoveRotationOffset, MoveRotationOffset);

			FVector deltaMove = FVector::XAxisVector.RotateAngleAxis(rotation, FVector::UpVector) * MoveDistance;
			formation->MoveFormationTo(deltaMove + center, rotation);
		}
	}
}

const FVector2D& AEnemyAI::GetFormationUnitOffset(int Amount, int Index) const
{
	Amount -= 1;

	if (Amount < 0 || Index < 0)
		return FVector2D::ZeroVector;

	if (Amount > FormationUnitOffsets.Num() - 1)
		Amount = FormationUnitOffsets.Num() - 1;

	if (Index > FormationUnitOffsets[Amount].UnitOffsets.Num() - 1) 
		return FVector2D::ZeroVector;

	return FormationUnitOffsets[Amount].UnitOffsets[Index];
}

void AEnemyAI::CreateFormationsTimeout()
{
	CreateFormations(AEnemyUnitController::GetEnemyControllers(), CombatFormationsAmount, 1000);
}

void AEnemyAI::CreateFormations(const TArray<AEnemyUnitController*>& Controllers, int FormationsAmount, int Iterations)
{
	if (FormationsAmount < 0)
		return;

	const static float RandomRange = 500.f;

	//K-Means clustering

	TArray<FVector2D> clusterMeanPoints;
	clusterMeanPoints.SetNum(FormationsAmount);

	for (int i = 0; i < FormationsAmount; i++)
	{
		const float x = FMath::RandRange(-RandomRange, RandomRange);
		const float y = FMath::RandRange(-RandomRange, RandomRange);

		clusterMeanPoints[i] = FVector2D(x, y);
	}

	for (int iter = 0; iter < Iterations; iter++)
	{
		TArray<TArray<FVector2D>> relatedClusterPoints;
		relatedClusterPoints.SetNum(FormationsAmount);

		//Find close points
		for (AEnemyUnitController* controller : Controllers)
		{
			if (!controller->GetCombatUnit())
				continue;

			const FVector2D location = (FVector2D)controller->GetCombatUnit()->GetActorLocation();
			int minDistanceIndex = 0;
			float minDistance = UE_MAX_FLT;

			for (int i = 0; i < FormationsAmount; i++)
			{
				const float distance = FVector2D::DistSquared(location, clusterMeanPoints[i]);

				if (distance < minDistance)
				{
					minDistanceIndex = i;
					minDistance = distance;
				}
			}

			relatedClusterPoints[minDistanceIndex].Add(location);
		}


		//Apply means
		for (int i = 0; i < FormationsAmount; i++)
		{
			const TArray<FVector2D>& points = relatedClusterPoints[i];

			if (points.Num() == 0)
				continue;

			FVector2D pointMean;

			for (const FVector2D& point : points)
			{
				pointMean += point;
			}

			pointMean /= points.Num();
			clusterMeanPoints[i] = pointMean;
		}
	}

	CombatFormations.Empty();
	CombatFormations.SetNum(FormationsAmount);

	for (UCombatFormation*& formation : CombatFormations)
	{
		formation = NewObject<UCombatFormation>(this);
	}

	//Find cluster for formation
	for (AEnemyUnitController* controller : Controllers)
	{
		if (!controller->GetCombatUnit())
			continue;

		const FVector2D location = (FVector2D)controller->GetCombatUnit()->GetActorLocation();

		int minDistanceIndex = 0;
		float minDistance = UE_MAX_FLT;

		for (int i = 0; i < FormationsAmount; i++)
		{
			const float distance = FVector2D::DistSquared(location, clusterMeanPoints[i]);

			if (distance < minDistance)
			{
				minDistanceIndex = i;
				minDistance = distance;
			}
		}

		CombatFormations[minDistanceIndex]->UnitControllers.Add(controller);
		controller->SetCombatFormation(CombatFormations[minDistanceIndex]);
	}

	CombatFormations = CombatFormations.FilterByPredicate([](const UCombatFormation* el) { return !(el->UnitControllers.IsEmpty()); });
}

float AEnemyAI::GetRotationToHQ(const FVector& Location)
{
	AHeadQuarters* hq = AHeadQuarters::GetInstance();

	if (!hq)
		return 180.f;

	return FRotator(FQuat::FindBetween(FVector::XAxisVector, hq->GetActorLocation() - Location)).Yaw;
}


