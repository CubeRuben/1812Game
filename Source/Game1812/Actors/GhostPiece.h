// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostPiece.generated.h"

UCLASS()
class GAME1812_API AGhostPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AGhostPiece();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* FoundationMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* FigureMeshComponent;

	virtual void BeginPlay() override;

public:	
	
	void CopyPiece(class APiece* Piece);
};
