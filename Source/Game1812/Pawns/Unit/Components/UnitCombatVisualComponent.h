#pragma once

#include "CoreMinimal.h"
#include "UnitVisualBaseComponent.h"
#include "UnitCombatVisualComponent.generated.h"

UCLASS()
class GAME1812_API UUnitCombatVisualComponent : public UUnitVisualBaseComponent
{
	GENERATED_BODY()

public:	

	UUnitCombatVisualComponent();

protected:

	class ACombatUnit* CombatUnitPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UUnitVisualMeshComponent*> UnitMeshComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UUnitVisualMeshComponent*> DeadUnitMeshComponents;

	TArray<class IFogAffected*> FogAffectedComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> MeshesOffsets;

	virtual void BeginPlay() override;

	void UpdateFormationOffsets(int Number, float Offset);

	void OnHealthPointsChange(float HealthPoints, bool WasDamaged);

	void ReviveMeshes(int NumberToRevive);
	void KillMeshes(int NumberToKill);
	void SpawnDeadMeshes(int Number);

public:	

	TArray<class IFogAffected*>* GetFogAffectedComponents();

	void UpdateVisual(float DeltaTime) override;

	void Init(class UCombatUnitDataAsset* UnitCombatStats);
		
};
