#pragma once

#include "CoreMinimal.h"
#include "StreetlightStyleConfiguration.h"
#include "Components/PointLightComponent.h"
#include "CesiumStreetlightComponent.generated.h"


UCLASS(ClassGroup="Cesium Streetlights", meta=(BlueprintSpawnableComponent))
class CESIUMSTREETLIGHTSFORUNREAL_API UCesiumStreetlightComponent : public UPointLightComponent
{
	GENERATED_BODY()

public:
	UCesiumStreetlightComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CesiumStreetlights")
	void ApplyStyle(const FStreetlightStyleConfiguration& configuration, class ACesiumGeoreference* georeference = nullptr);
};
