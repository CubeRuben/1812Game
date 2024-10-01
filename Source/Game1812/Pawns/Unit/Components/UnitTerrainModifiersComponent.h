#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitTerrainModifiersComponent.generated.h"


USTRUCT(BlueprintType)
struct FUnitTerrainModifiers 
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeedModifier = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeedModifier = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageModifier = 1.f;

	void Reset(float Value = 1.f);
};

UCLASS()
class GAME1812_API UUnitTerrainModifiersComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UUnitTerrainModifiersComponent();

protected:

	class ABaseUnit* OwnerUnit;

	UPROPERTY(EditAnywhere)
	TSet<class ATerrainPropertyVolume*> OverlappedTerrainVolumes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUnitTerrainModifiers Modifiers;

	void BeginPlay() override;

	void UpdateModifiers();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

	const FUnitTerrainModifiers& GetTerrainModifiers() const { return Modifiers; }

};
