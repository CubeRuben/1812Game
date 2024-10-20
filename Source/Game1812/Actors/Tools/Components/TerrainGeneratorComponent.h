#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TerrainGeneratorComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Common), meta = (BlueprintSpawnableComponent))
class GAME1812_API UTerrainGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UTerrainGeneratorComponent();

protected:

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	class ATerrainPropertyVolume* OwnerVolume;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TSet<TObjectPtr<UStaticMeshComponent>> PropsMeshComponents;

	UPROPERTY(EditAnywhere)
	class UTerrainGeneratorDataAsset* GeneratorDataAsset;

	UPROPERTY(EditAnywhere)
	int Seed;

	virtual void OnRegister() override;

public:	



	void GenerateProps();
	void ClearProps();


#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif
	
};
