#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeadQuarters.generated.h"

UCLASS()
class GAME1812_API AHeadQuarters : public AActor
{
	GENERATED_BODY()
	
public:	

	AHeadQuarters();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UOrdersSenderComponent* OrdersSenderComponent;

	void BeginPlay() override;
};
