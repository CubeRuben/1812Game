#pragma once

#include "CoreMinimal.h"
#include "UnitVisualBaseComponent.h"
#include "UnitSingleVisualComponent.generated.h"

UCLASS()
class GAME1812_API UUnitSingleVisualComponent : public UUnitVisualBaseComponent
{
	GENERATED_BODY()
	
public:

	UUnitSingleVisualComponent();

protected:

	class ABaseUnit* UnitPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UUnitVisualMeshComponent* VisualMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* VisualMesh;

	void BeginPlay() override;

public:

	void UpdateVisual(float DeltaTime) override;

	void Init();

};
