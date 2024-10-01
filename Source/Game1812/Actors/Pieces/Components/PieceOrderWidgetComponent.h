#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PieceOrderWidgetComponent.generated.h"

UCLASS(BlueprintType)
class GAME1812_API UPieceOrderWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UPieceOrderWidgetComponent();

protected:

	UPROPERTY(BlueprintReadOnly)
	class APiece* OwnerPiece;

	virtual void BeginPlay() override;

	void OnEnable();
	void OnDisable();

	UFUNCTION(BlueprintCallable)
	void AssignOrder(class UUnitOrder* UnitOrder);

public:

	
};
