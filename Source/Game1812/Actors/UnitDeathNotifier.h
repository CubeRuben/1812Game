#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../FogSystem/FogAffected.h"
#include "UnitDeathNotifier.generated.h"

UCLASS()
class GAME1812_API AUnitDeathNotifier : public AActor, public IFogAffected
{
	GENERATED_BODY()
	
public:	

	AUnitDeathNotifier();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class APiece> PieceToNotify;

public:	

	void SetPiece(class APiece* NewPiece);

	//IFogAffected Interface
	void OnBeingCoveredInFog() override;
	void OnBeingRevealedFromFog() override;
	bool IsCoveredInFog() override;
	//

};
