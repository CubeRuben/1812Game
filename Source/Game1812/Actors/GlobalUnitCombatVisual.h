#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalUnitCombatVisual.generated.h"

UCLASS()
class GAME1812_API AGlobalUnitCombatVisual : public AActor
{
	GENERATED_BODY()
	
public:	

	AGlobalUnitCombatVisual();

protected:

	static AGlobalUnitCombatVisual* Instance;

	TArray<TWeakObjectPtr<class UUnitCombatVisualComponent>> VisualComponents;

	TArray<TWeakObjectPtr<class UUnitCombatMeshComponent>> MeshComponents;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CrammingDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CrammingDistanceSquared;

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:	

	static AGlobalUnitCombatVisual* GetInstance() { return Instance; }
	static AGlobalUnitCombatVisual* GetInstance(UWorld* World);

	void AddVisualComponent(class UUnitCombatVisualComponent* NewVisualComponent, const TArray<class UUnitCombatMeshComponent*>& NewMeshComponents);
	void RemoveVisualComponent(class UUnitCombatVisualComponent* VisualComponentToRemove);

	virtual void Tick(float DeltaTime) override;

};
