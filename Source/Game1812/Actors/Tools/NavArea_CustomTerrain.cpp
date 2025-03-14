#include "NavArea_CustomTerrain.h"

UNavArea_CustomTerrain::UNavArea_CustomTerrain()
{
	DefaultCost = 0.0f;
	DrawColor = FColor::Red;
}

UNavArea_CustomTerrain_River::UNavArea_CustomTerrain_River()
{
	DefaultCost = 15.0f;
	DrawColor = FColor::Blue;
}

UNavArea_CustomTerrain_ForestDense::UNavArea_CustomTerrain_ForestDense()
{
	DefaultCost = 5.0f;
	DrawColor = FColor::Green;
}

UNavArea_CustomTerrain_ForestSparse::UNavArea_CustomTerrain_ForestSparse()
{
	DefaultCost = 0.5f;
	DrawColor = FColor::Green;
}

UNavArea_CustomTerrain_Grass::UNavArea_CustomTerrain_Grass()
{
	DefaultCost = 0.5f;
	DrawColor = FColor::Green;
}

UNavArea_CustomTerrain_Swamp::UNavArea_CustomTerrain_Swamp()
{
	DefaultCost = 7.0f;
	DrawColor = FColor::Green;
}
