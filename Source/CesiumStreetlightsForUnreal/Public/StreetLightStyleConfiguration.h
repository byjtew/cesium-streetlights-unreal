#pragma once

#include "CoreMinimal.h"
#include "Engine/Scene.h"
#include "StreetLightStyleConfiguration.generated.h"

USTRUCT(BlueprintType)
struct CESIUMSTREETLIGHTSFORUNREAL_API FStreetLightStyleConfiguration
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FColor Color = FColor::Yellow;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  float RangeMeters = 50.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  float Intensity = 1000.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  ELightUnits LightUnits = ELightUnits::Lumens;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  bool bCastShadows = false;
};
