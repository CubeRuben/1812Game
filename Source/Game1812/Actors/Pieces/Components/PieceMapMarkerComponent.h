#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "PieceMapMarkerComponent.generated.h"

UCLASS()
class GAME1812_API UPieceMapMarkerComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:

	UPieceMapMarkerComponent();

protected:

	class APiece* OwnerPiece;

	void BeginPlay() override;

	void UpdateLocation();
	void UpdateRotation();

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void OnEnable();
	void OnDisable();
};
