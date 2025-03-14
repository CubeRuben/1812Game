#pragma once

#include "CoreMinimal.h"
#include "NavAreas/NavArea.h"
#include "NavArea_CustomTerrain.generated.h"

UCLASS(Blueprintable)
class GAME1812_API UNavArea_CustomTerrain : public UNavArea
{
	GENERATED_BODY()
	
public:

	UNavArea_CustomTerrain();
};

UCLASS(Blueprintable)
class GAME1812_API UNavArea_CustomTerrain_River : public UNavArea_CustomTerrain
{
	GENERATED_BODY()

public:

	UNavArea_CustomTerrain_River();
};

UCLASS(Blueprintable)
class GAME1812_API UNavArea_CustomTerrain_ForestDense : public UNavArea_CustomTerrain
{
	GENERATED_BODY()

public:

	UNavArea_CustomTerrain_ForestDense();
};

UCLASS(Blueprintable)
class GAME1812_API UNavArea_CustomTerrain_ForestSparse : public UNavArea_CustomTerrain
{
	GENERATED_BODY()

public:

	UNavArea_CustomTerrain_ForestSparse();
};

UCLASS(Blueprintable)
class GAME1812_API UNavArea_CustomTerrain_Grass : public UNavArea_CustomTerrain
{
	GENERATED_BODY()

public:

	UNavArea_CustomTerrain_Grass();
};

UCLASS(Blueprintable)
class GAME1812_API UNavArea_CustomTerrain_Swamp : public UNavArea_CustomTerrain
{
	GENERATED_BODY()

public:

	UNavArea_CustomTerrain_Swamp();
};