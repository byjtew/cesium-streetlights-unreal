#include "CesiumStreetlightComponent.h"

#include "internal/CesiumExtensions.h"


UCesiumStreetlightComponent::UCesiumStreetlightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCesiumStreetlightComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCesiumStreetlightComponent::ApplyStyle(const FStreetlightStyleConfiguration& configuration, ACesiumGeoreference* georeference)
{
	SetIntensityUnits(ELightUnits::Lumens);
	SetIntensity(configuration.Intensity);
	SetLightColor(configuration.GetLightColor());
	SetSourceRadius(cesium_ext::MetersToUnreal(configuration.SourceRadius, georeference));
	SetAttenuationRadius(cesium_ext::MetersToUnreal(configuration.AttenuationRadius, georeference));
	SetCastShadows(configuration.CastShadows);
}
