#include "UnitPieceProjectionComponent.h"

#include "../../../CossacksGameInstance.h"
#include "../Units/CombatUnit.h"
#include "../../../Actors/EnemyPiece.h"

UUnitPieceProjectionComponent::UUnitPieceProjectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UUnitPieceProjectionComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatUnitPawn = Cast<ACombatUnit>(GetOwner());

	if (!CombatUnitPawn)
		DestroyComponent();

	CombatUnitPawn->OnUnitFogChange.AddUObject(this, &UUnitPieceProjectionComponent::OnUnitFogChange);
}

void UUnitPieceProjectionComponent::OnUnitFogChange(bool IsCoveredInFog)
{
	if (IsCoveredInFog || Piece)
		return;

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UCossacksGameInstance* gameInstance = GetWorld()->GetGameInstance<UCossacksGameInstance>();

	if (!gameInstance)
		return;

	Piece = GetWorld()->SpawnActor<AEnemyPiece>(gameInstance->GetEnemyPieceClass(), params);

	if (Piece)
		Piece->SetCombatUnit(CombatUnitPawn);
}
