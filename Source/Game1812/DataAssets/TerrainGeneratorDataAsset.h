#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TerrainGeneratorDataAsset.generated.h"

UCLASS()
class GAME1812_API UTerrainGeneratorDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UTerrainGeneratorDataAsset();

protected:

	UPROPERTY(EditAnywhere)
	float MinDistance;

	UPROPERTY(EditAnywhere)
	float FalloffPower;

	UPROPERTY(EditAnywhere)
	bool RandomRotation;

	UPROPERTY(EditAnywhere)
	FVector Scale;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> MeshVariants;

public:

	float GetMinDistance() const { return MinDistance; };
	float GetFalloffPower() const { return FalloffPower; };
	bool GetRandomRotation() const { return RandomRotation; };
	const FVector& GetScale() const { return Scale; };
	const TArray<UStaticMesh*>& GetMeshVariants() const { return MeshVariants;};
	
};
