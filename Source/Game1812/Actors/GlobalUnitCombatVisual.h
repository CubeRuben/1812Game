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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UUnitCombatVisualComponent*> VisualComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UUnitCombatMeshComponent*> MeshComponents;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CrammingDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CrammingDistanceSquared;

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:	

	static AGlobalUnitCombatVisual* GetInstance() { return Instance; }
	static void CreateInstance(UWorld* World);

	void AddVisualComponents(class UUnitCombatVisualComponent* NewVisualComponent, const TArray<class UUnitCombatMeshComponent*>& NewMeshComponents);

	virtual void Tick(float DeltaTime) override;

};
