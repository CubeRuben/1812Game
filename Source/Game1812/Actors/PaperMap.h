#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperMap.generated.h"

UCLASS()
class GAME1812_API APaperMap : public AActor
{
	GENERATED_BODY()
	
public:	

	APaperMap();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* PaperMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BordersComponent;

	virtual void BeginPlay() override;


};
