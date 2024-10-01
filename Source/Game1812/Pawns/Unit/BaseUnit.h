#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../../FogSystem/FogAffected.h"
#include "TeamEnum.h"
#include "UnitTypeEnum.h"
#include "BaseUnit.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitFogChangeDelegate, bool);

UCLASS(Abstract, Blueprintable)
class GAME1812_API ABaseUnit : public APawn, public IFogAffected
{
	GENERATED_BODY()

public:

	ABaseUnit();

protected:
	
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UUnitMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UUnitTerrainModifiersComponent* TerrainModifiersComponent;
	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class APiece> OwnerPiece;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETeam Team;

	virtual void BeginPlay() override;

public:	

	FOnUnitFogChangeDelegate OnUnitFogChange;

	virtual void OnUnitDeath();

	virtual void AssignOrder(class UUnitOrder* NewOrder) {};

	//IFogAffected Interface
	void OnBeingCoveredInFog() override;
	void OnBeingRevealedFromFog() override;
	bool IsCoveredInFog() override;
	//

	virtual class UBoxComponent* GetColliderComponent() { return BoxComponent; };
	virtual class UUnitMovementComponent* GetMovementComponent() { return nullptr; };

	virtual float GetMovementSpeed() const { return 0.f; };
	virtual float GetRotationSpeed() const { return 0.f; };

	virtual class UUnitOrder* GetCurrentOrder() { return nullptr; };

	virtual ETeam GetTeam() const { return Team; };
	virtual ECombatUnitType GetUnitType() const { return ECombatUnitType::None; };

	const struct FUnitTerrainModifiers& GetTerrainModifiers() const;

	void SetOwnerPiece(class APiece* NewOwnerPiece);

};
