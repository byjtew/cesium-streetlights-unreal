#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StreetlightStyleConfiguration.h"
#include "CesiumStreetlightsGeodeskVolumeActor.generated.h"


class ACesiumStreetlightsVolumeActor;
class UCesiumStreetlightComponent;

UCLASS(ClassGroup="Cesium Streetlights")
class CESIUMSTREETLIGHTSFORUNREAL_API ACesiumStreetlightsGeodeskVolumeActor : public AActor
{
	GENERATED_BODY()

public:
	ACesiumStreetlightsGeodeskVolumeActor();

	virtual void BeginPlay() override;
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(
		CallInEditor, BlueprintCallable,
		Category = "CesiumStreetlights"
	)
	void Generate();

	UFUNCTION(
		CallInEditor, BlueprintCallable,
		Category = "CesiumStreetlights"
	)
	void DryRun();

	UFUNCTION(
		CallInEditor, BlueprintCallable,
		Category = "CesiumStreetlights"
	)
	void Reset();

	UFUNCTION(
		CallInEditor, BlueprintCallable,
		Category = "CesiumStreetlights",
		meta=(ToolTip="Update the style of all currently spawned streetlights to match the first style configuration.")
	)
	void UpdateCurrentLightsStyle();

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(
		EditAnywhere, BlueprintReadWrite,
		Category="Cesium Streetlights",
		meta=(ToolTip="The Cesium tileset actor that will be sampled to obtain correct altitudes.")
	)
	class ACesium3DTileset* GroundTileset = nullptr;

	UPROPERTY(
		EditAnywhere, BlueprintReadWrite,
		Category="Cesium Streetlights"
	)
	FFilePath GolFilepath;

	UPROPERTY(
		EditAnywhere, BlueprintReadWrite,
		Category="Cesium Streetlights",
		meta=(ToolTip="Radius in meters around the owning actor in which to spawn the streetlights", ClampMin="0", UIMin="0", Units=Meters)
	)
	double Radius = 500.0;

	UPROPERTY(
		EditAnywhere, BlueprintReadWrite,
		Category="Cesium Streetlights",
		meta=(ToolTip="Style configurations to apply to streetlights in this volume. The priority will follow the order of this array.")
	)
	TArray<FStreetlightStyleConfiguration> StyleConfigurations = { FStreetlightStyleConfiguration() };

	UPROPERTY()
	TArray<UCesiumStreetlightComponent*> SpawnedStreetlights;

protected:
	struct FGeodeskRequestsConfiguration
	{
		FString GolFilepath;

		TArray<FString> GoqlRequests;

		/**
		 * Optional: the origin [longitude,latitude] (in degrees) and radius (in meters) of the area.
		 */
		TOptional<TTuple<FVector2d, double>> OriginAndRadius;
	};

protected:
	UCesiumStreetlightComponent*   SpawnStreetlightComponent() const;
	static TArray<TArray<FVector>> LoadGeoPositionsFromGolFile(FGeodeskRequestsConfiguration requests);
};
