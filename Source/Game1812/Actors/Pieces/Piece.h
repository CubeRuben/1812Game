#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Pawns/Player/Components/Interactable.h"
#include "../../Pawns/Unit/BaseUnit.h"
#include "../../Macros/EventDelegate.h"
#include <Blueprint/UserWidget.h>
#include "Piece.generated.h"

DECLARE_MULTICAST_DELEGATE(FPieceEventDelegate);

#define ADD_PIECE_EVENT_DELEGATE(name) TEMPLATE_EVENT_DELEGATE(FPieceEventDelegate, name)

UENUM(BlueprintType)
enum class EPieceSoundType : uint8 
{
	StartDragging,
	Selected,
	Death
};

UCLASS()
class GAME1812_API APiece : public AActor, public IInteractable
{
	GENERATED_BODY()

public:

	APiece();

protected:

	//
	// Scene Components
	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* PieceFoundationMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* PieceFigureMeshComponent;

	//
	// Actor Components
	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPieceMapMarkerComponent* MapMarkerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPieceOrderWidgetComponent* OrderWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPiecePredictedPathComponent* PredictedPathComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPieceOutlineComponent* OutlineComponent;

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class ABaseUnit> Unit;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseUnit> UnitClass;

	UPROPERTY(VisibleAnywhere)
	bool bWasDragged;

	UPROPERTY(VisibleAnywhere)
	bool bCanSpawnUnit;

	UPROPERTY(VisibleAnywhere)
	bool bIsDead;

	// 
	// Delegates
	//

	ADD_PIECE_EVENT_DELEGATE(UnitSpawn);
	ADD_PIECE_EVENT_DELEGATE(UnitDeath);

	ADD_PIECE_EVENT_DELEGATE(OrderAssign);

	ADD_PIECE_EVENT_DELEGATE(MapHit);
	ADD_PIECE_EVENT_DELEGATE(MapHitWasDragged);
	ADD_PIECE_EVENT_DELEGATE(RemovedFromMap);

	// Delegates - Interaction

	ADD_PIECE_EVENT_DELEGATE(StartDragging);
	ADD_PIECE_EVENT_DELEGATE(StopDragging);
	ADD_PIECE_EVENT_DELEGATE(StartCursorHover);
	ADD_PIECE_EVENT_DELEGATE(StopCursorHover);
	ADD_PIECE_EVENT_DELEGATE(StartGroupSelectionHover);
	ADD_PIECE_EVENT_DELEGATE(StopGroupSelectionHover);
	ADD_PIECE_EVENT_DELEGATE(Selected);
	ADD_PIECE_EVENT_DELEGATE(SelectionRemoved);

	//

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SpawnUnit();

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySound(EPieceSoundType SoundType);

public:

	virtual void Tick(float DeltaTime) override;

	bool IsDead() const { return bIsDead; }

	//Return true if has related unit and it's not dead
	bool IsPlayed() const { return !bCanSpawnUnit && !bIsDead; }

	UStaticMeshComponent* GetPieceFoundationMeshComponent() const { return PieceFoundationMeshComponent; }
	UStaticMeshComponent* GetPieceFigureMeshComponent() const { return PieceFigureMeshComponent; }

	class ABaseUnit* GetUnit();

	UFUNCTION(BlueprintCallable)
	virtual void AssignOrder(class UUnitOrder* UnitOrder);

	virtual void OnDeathUnit();

	FRotator GetResetRotation();
	void ResetRotation();

	//
	// IInteractable Interface
	//
	virtual void StartDragging() override;
	virtual void StopDragging() override;

	virtual void StartCursorHover() override;
	virtual void StopCursorHover() override;

	virtual void StartGroupSelectionHover() override;
	virtual void StopGroupSelectionHover() override;

	virtual void Selected() override;
	virtual void SelectionRemoved() override;

	FVector GetDragOffset() override;

	virtual bool CanBeGrouped() override { return true; }
	//
};
