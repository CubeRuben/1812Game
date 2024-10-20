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
	bool RandomRotation;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> MeshVariants;

public:

	float GetMinDistance() const { return MinDistance; };
	bool GetRandomRotation() const { return RandomRotation; };
	const TArray<UStaticMesh*>& GetMeshVariants() const { return MeshVariants;};
	
};
