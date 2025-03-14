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

	virtual void OnConstruction(const FTransform& Transform) override;

public:

	const class UTerrainPropertyDataAsset* GetTerrainPropertyDataAsset() const { return TerrainPropertyDataAsset; };
	class UBoxComponent* GetBoxComponent() const { return BoxComponent; };

};
