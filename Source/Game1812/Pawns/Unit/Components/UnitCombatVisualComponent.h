#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitCombatVisualComponent.generated.h"

UCLASS()
class GAME1812_API UUnitCombatVisualComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UUnitCombatVisualComponent();

protected:

	class ACombatUnit* CombatUnitPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UStaticMeshComponent*> UnitMeshComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> MeshesOffsets;

	virtual void BeginPlay() override;

	void UpdateFormationOffsets(int Number, float Offset);

public:	

	void UpdateVisual(float DeltaTime);

	void Init(class UCombatUnitDataAsset* UnitCombatStats);
		
};
