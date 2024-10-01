#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainPropertyVolume.generated.h"

UCLASS()
class GAME1812_API ATerrainPropertyVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ATerrainPropertyVolume();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) 
	class UNavModifierComponent* NavModifierComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTerrainPropertyDataAsset* TerrainPropertyDataAsset;

public:

	const class UTerrainPropertyDataAsset* GetTerrainPropertyDataAsset() const { return TerrainPropertyDataAsset; };

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
