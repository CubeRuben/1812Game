#pragma once

#include "CoreMinimal.h"
#include "PieceOrderWidgetComponent.h"
#include "PieceScoutOrderWidgetComponent.generated.h"

UCLASS()
class GAME1812_API UPieceScoutOrderWidgetComponent : public UPieceOrderWidgetComponent
{
	GENERATED_BODY()

public:

	UPieceScoutOrderWidgetComponent();

	void AssignOrder(class UUnitOrder* UnitOrder) override;
	
};
