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
	TArray<class UUnitCombatMeshComponent*> UnitMeshComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UUnitCombatMeshComponent*> DeadUnitMeshComponents;

	TArray<class IFogAffected*> FogAffectedComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> MeshesOffsets;

	virtual void BeginPlay() override;

	void UpdateFormationOffsets(int Number, float Offset);

	void OnHealthPointsChange(float HealthPoints);

	void ReviveMeshes(int NumberToRevive);
	void KillMeshes(int NumberToKill);
	void SpawnDeadMeshes(int Number);

public:	

	TArray<class IFogAffected*>* GetFogAffectedComponents();

	void UpdateVisual(float DeltaTime);

	void Init(class UCombatUnitDataAsset* UnitCombatStats);
		
};
