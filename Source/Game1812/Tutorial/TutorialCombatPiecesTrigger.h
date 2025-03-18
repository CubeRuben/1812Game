#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Macros/EventDelegate.h"
#include "TutorialCombatPiecesTrigger.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FCombatPieceOverlapDelegate, class ACombatPiece*);

#define ADD_COMBAT_PIECE_OVERLAP_EVENT_DELEGATE(name) TEMPLATE_EVENT_DELEGATE(FCombatPieceOverlapDelegate, name)

UCLASS()
class GAME1812_API ATutorialCombatPiecesTrigger : public AActor
{
	GENERATED_BODY()
	
public:	

	ATutorialCombatPiecesTrigger();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* TriggerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	ADD_COMBAT_PIECE_OVERLAP_EVENT_DELEGATE(PieceOverlapBegin);
	ADD_COMBAT_PIECE_OVERLAP_EVENT_DELEGATE(PieceOverlapEnd);

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

	bool GetTriggerActive() const;
	void SetTriggerActive(bool NewActive);

};
