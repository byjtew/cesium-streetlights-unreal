#pragma once

#include "CoreMinimal.h"
#include "Misc/Optional.h"
#include "StreetLightSpawnConfiguration.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CESIUMSTREETLIGHTSFORUNREAL_API FStreetLightSpawnConfiguration
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CesiumStreetLights")
  float SpawnRadiusMeters = 10.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CesiumStreetLights")
  int32 MaxNumberOfLights = 100;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CesiumStreetLights")
  TOptional<double> SpaceBetweenLightsMeters = TOptional(35.0);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CesiumStreetLights")
  float SpawnHeightMeters = 5.0f;
};
