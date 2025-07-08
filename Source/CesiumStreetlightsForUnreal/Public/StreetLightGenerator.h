#pragma once

#include "CoreMinimal.h"
#include "StreetLightSpawnConfiguration.h"
#include "StreetLightStyleConfiguration.h"
#include "GameFramework/Actor.h"
#include "Misc/Paths.h"
#include "StreetLightGenerator.generated.h"

UCLASS()
class CESIUMSTREETLIGHTSFORUNREAL_API AStreetLightGenerator : public AActor
{
  GENERATED_BODY()

public:
  AStreetLightGenerator();

  virtual void BeginPlay() override;

  virtual bool ShouldTickIfViewportsOnly() const override;

  virtual void Tick(float DeltaTime) override;

  UFUNCTION(CallInEditor, BlueprintCallable, Category="CesiumStreetLights")
  void GenerateStreetLights();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CesiumStreetLights")
  FFilePath OsmFilepath;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CesiumStreetLights")
  FStreetLightSpawnConfiguration SpawnConfiguration;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CesiumStreetLights")
  FStreetLightStyleConfiguration StyleConfiguration;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CesiumStreetLights")
  TSubclassOf<AActor> StreetLightActorClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CesiumStreetLights")
  float DebugDrawLightSizeMeters = 50.0f;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CesiumStreetLights")
  TArray<AActor*> SpawnedStreetLights;

protected:
  AActor* SpawnLightAtLocation(const FVector& Location);
};
