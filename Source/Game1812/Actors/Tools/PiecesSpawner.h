#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Pawns/Unit/TeamEnum.h"
#include "PiecesSpawner.generated.h"

UENUM()
enum class EPieceToSpawn : uint8 {
	COMBAT = 0	UMETA(DisplayName = "Combat Piece"),
	SCOUT = 1	UMETA(DisplayName = "Scout Piece"),
};

UCLASS()
class GAME1812_API APiecesSpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	APiecesSpawner();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPieceToSpawn PieceToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "PieceToSpawn==EPieceToSpawn::Combat"))
	class UCombatUnitDataAsset* CombatUnitData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnOffset;

	UFUNCTION(BlueprintCallable)
	void SpawnPieces();
	void SpawnPiece(const FVector& Location);
};
