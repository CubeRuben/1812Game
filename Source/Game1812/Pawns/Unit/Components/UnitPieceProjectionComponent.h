#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitPieceProjectionComponent.generated.h"


UCLASS()
class GAME1812_API UUnitPieceProjectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UUnitPieceProjectionComponent();

protected:

	class ACombatUnit* CombatUnitPawn;

	class AEnemyPiece* Piece;

	virtual void BeginPlay() override;

	void OnUnitFogChange(bool IsCoveredInFog);

public:	
		
};
