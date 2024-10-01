#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Pawns/Player/Components/Interactable.h"
#include "../Pawns/Unit/Reports/UnitReport.h"
#include "Report.generated.h"

UCLASS()
class GAME1812_API AReport : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	AReport();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PaperMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FUnitReport Report;

public:	

	
	void SetReportData(const FUnitReport& ReportData);

	UFUNCTION(BlueprintNativeEvent)
	void DisplayReportData();

	//IDraggable Interface
	void StartDragging() override;
	void StopDragging() override;

	void StartCursorHover() override;
	void StopCursorHover() override;

	virtual void StartGroupSelectionHover() override;
	virtual void StopGroupSelectionHover() override;

	void Selected() override;
	void SelectionRemoved() override;

	FVector GetDragOffset() override;

	bool CanBeGrouped() override { return false; }
	//
};
