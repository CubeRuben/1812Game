#include "FogOfWar.h"

#include "FogAffected.h"
#include "../Actors/HeadQuarters.h"

#include <Components/BoxComponent.h>
#include <Components/DecalComponent.h>
#include <Kismet/GameplayStatics.h>

FFogDiscoveredArea::FFogDiscoveredArea()
{
	DiscoverEndTime = 0.f;
}

FFogDiscoveredArea::FFogDiscoveredArea(const FFogDiscoveredArea& Other)
{
	DiscoveredArea = Other.DiscoveredArea;
	DiscoverEndTime = Other.DiscoverEndTime;
}

FFogDiscoveredArea::FFogDiscoveredArea(const TImageBuilder<FVector4f>& Area, float Time)
{
	DiscoveredArea = TImageBuilder<FVector4f>(Area);
	DiscoverEndTime = Time;
}

//bool operator==(const FFogDiscoveredArea& First, const FFogDiscoveredArea& Second)
//{
//	return First.TextureBuilder == Second.TextureBuilder;
//}

AFogOfWar* AFogOfWar::Instance = nullptr;

AFogOfWar* AFogOfWar::GetInstance()
{
	return Instance;
}

AFogOfWar::AFogOfWar()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	FogArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Fog Area Component"));
	FogArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FogArea->SetupAttachment(RootComponent);

	FogDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Fog Decal Component"));
	FogDecalComponent->SetupAttachment(RootComponent);

	AffectActors = true;

	HeadQuartersRange = 8.0f;
	ScoutRange = 5.0f;
	ScoutRevealTime = 50.0f;
}

void AFogOfWar::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;
	Resolution = FIntPoint(64, 64);

	const FImageDimensions dimensions(Resolution.X, Resolution.Y);

	ConstantDiscoveredArea.SetDimensions(dimensions);
	ConstantDiscoveredArea.Clear(FVector4f::Zero());

	FogAlphaImageBuilder.SetDimensions(dimensions);
	FogAlphaImageBuilder.Clear(FVector4f::Zero());

	FogDynamicMaterial = UMaterialInstanceDynamic::Create(FogMaterialAsset, this);
	FogDecalComponent->SetDecalMaterial(FogDynamicMaterial);

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &AFogOfWar::AddConstantDiscoveredArea));
}

void AFogOfWar::AddConstantDiscoveredArea()
{
	if (AHeadQuarters::GetInstance())
		ApplyCircularBrushToImage(ConstantDiscoveredArea, LocationToIndex(AHeadQuarters::GetInstance()->GetActorLocation()), HeadQuartersRange, FVector4f::One());

	UpdateFogTexture();
}

void AFogOfWar::UpdateFogTexture()
{
	FogAlphaImageBuilder.Clear(FVector4f::Zero());

	AddTextureToTexture(FogAlphaImageBuilder, ConstantDiscoveredArea);

	for (FFogDiscoveredArea& area : TimedDiscoveredAreas)
	{
		AddTextureToTexture(FogAlphaImageBuilder, area.DiscoveredArea);
	}

	FTexture2DBuilder fogAlphaTextureBuilder;
	fogAlphaTextureBuilder.Initialize(FTexture2DBuilder::ETextureType::ColorLinear, FImageDimensions(Resolution.X, Resolution.Y));
	fogAlphaTextureBuilder.Copy(FogAlphaImageBuilder);
	fogAlphaTextureBuilder.Commit(false);
	FogAlphaTexture = fogAlphaTextureBuilder.GetTexture2D();

	FogDynamicMaterial->SetTextureParameterValue("AlphaTexture", FogAlphaTexture);
}

void AFogOfWar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateDiscoveredAreas();

	CheckActorsInFog();
}

void AFogOfWar::CheckActorsInFog()
{
	TArray<AActor*> affectedActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UFogAffected::StaticClass(), affectedActors);

	for (AActor* actor : affectedActors)
	{
		if (!actor)
			continue;

		IFogAffected* fogAffected = Cast<IFogAffected>(actor);

		if (!fogAffected)
			continue;

		FIntPoint index = LocationToIndex(actor->GetActorLocation());

		const bool isFogDisabled = FogAlphaImageBuilder.GetPixel(FVector2i(index.X, index.Y)).W > 0.75f;
		const bool isActorInFog = fogAffected->IsCoveredInFog();

		if (isFogDisabled && isActorInFog || !AffectActors)
		{
			fogAffected->OnBeingRevealedFromFog();
		}
		else if (!isFogDisabled && !isActorInFog)
		{
			fogAffected->OnBeingCoveredInFog();
		}
	}
}

void AFogOfWar::UpdateDiscoveredAreas()
{
	const float timeSeconds = GetWorld()->GetTimeSeconds();
	bool shouldUpdateTexture = false;

	for (int i = 0; i < TimedDiscoveredAreas.Num();)
	{
		if (TimedDiscoveredAreas[i].DiscoverEndTime < timeSeconds) 
		{
			TimedDiscoveredAreas.RemoveAt(i);
			shouldUpdateTexture = true;
		}
		else 
		{
			i++;
		}
	}

	if (shouldUpdateTexture)
		UpdateFogTexture();
}

FVector AFogOfWar::GetChunkSize()
{
	const FVector AreaSize = FogArea->GetScaledBoxExtent() * 2;
	const FVector SystemSize(AreaSize.X / Resolution.X, AreaSize.Y / Resolution.Y, AreaSize.Z);
	return SystemSize;
}

FIntPoint AFogOfWar::LocationToIndex(FVector Location)
{
	const FVector ChunkSize = GetChunkSize();
	const FVector RelativeLocation = Location - GetActorLocation() + FogArea->GetScaledBoxExtent();
	FIntPoint ret(RelativeLocation.X / ChunkSize.X, RelativeLocation.Y / ChunkSize.Y);

	ret.X = FMath::Clamp(ret.X, 0, Resolution.X - 1);
	ret.Y = FMath::Clamp(ret.Y, 0, Resolution.Y - 1);

	return ret;
}

void AFogOfWar::AddDiscoveredArea(const TImageBuilder<FVector4f>& Area)
{
	TimedDiscoveredAreas.Add(FFogDiscoveredArea(Area, GetWorld()->GetTimeSeconds() + ScoutRevealTime));

	UpdateFogTexture();
}

void AFogOfWar::AddTextureToTexture(TImageBuilder<FVector4f>& MainImage, const TImageBuilder<FVector4f>& Image)
{
	if (MainImage.GetDimensions() != Image.GetDimensions())
		return;

	for (int x = 0; x < MainImage.GetDimensions().GetWidth(); x++)
	{
		for (int y = 0; y < MainImage.GetDimensions().GetHeight(); y++)
		{
			const FVector2i coords(x, y);
			const FVector4f newColor = MainImage.GetPixel(coords) + Image.GetPixel(coords);
			MainImage.SetPixel(coords, newColor);
		}
	}
}

void ApplyCircularBrushToImage(TImageBuilder<FVector4f>& Image, FIntPoint Position, float Radius, FVector4f Color)
{
	const int xStart = FMath::Max(Position.X - Radius, 0);
	const int xEnd = FMath::Min(Image.GetDimensions().GetWidth() - 1, Position.X + Radius);
	const int yStart = FMath::Max(Position.Y - Radius, 0);
	const int yEnd = FMath::Min(Image.GetDimensions().GetHeight() - 1, Position.Y + Radius);

	const float squaredRadius = Radius * Radius;

	for (int x = xStart; x <= xEnd; x++)
	{
		for (int y = yStart; y <= yEnd; y++)
		{
			const float squaredDistance = FVector2D::DistSquared(Position, FVector2D(x, y));

			if (squaredDistance < squaredRadius)
			{
				Image.SetPixel(FVector2i(x, y), Color);
			}
		}
	}
}
