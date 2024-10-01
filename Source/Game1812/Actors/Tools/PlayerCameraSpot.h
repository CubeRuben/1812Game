#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCameraSpot.generated.h"

UCLASS()
class GAME1812_API APlayerCameraSpot : public AActor
{
	GENERATED_BODY()
	
public:	

	APlayerCameraSpot();

	bool operator<(const APlayerCameraSpot& other) const { return Priority < other.Priority; }

protected:

	UPROPERTY(EditAnywhere, Category = "Spot Settings")
	int Priority;

	UPROPERTY(EditAnywhere, Category = "Spot Settings")
	float LocationInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Spot Settings")
	float RotationInterpSpeed;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

public: 

	int GetPriority() { return Priority; }

	bool HaveCustomLocationInterpSpeed() { return LocationInterpSpeed != 0; }
	bool HaveCustomRotationInterpSpeed() { return RotationInterpSpeed != 0; }

	float GetLocationInterpSpeed() { return LocationInterpSpeed; }
	float GetRotationInterpSpeed() { return RotationInterpSpeed; }

};
