#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StreetlightStyleConfiguration.generated.h"

/**
 * A struct to hold a key and multiple values for that key.
 * Hack to do that:
 * ```
 * UPROPERTY(...)
 * TMap<FString, TArray<FString>> Tags;
 * ```
 */
USTRUCT(BlueprintType)
struct CESIUMSTREETLIGHTSFORUNREAL_API FTagValuesArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cesium Streetlights")
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cesium Streetlights")
	TArray<FString> Values;
};


/**
 * Configuration for a set of streetlights (motorways/residential/alleys/railways/etc.)
 */
USTRUCT(BlueprintType)
struct CESIUMSTREETLIGHTSFORUNREAL_API FStreetlightStyleConfiguration
{
	GENERATED_BODY()

public:
	void Preset_Default();
	void Preset_Motorway();
	void Preset_Residential();
	void Preset_Alley();
	void Preset_Railway();

	FColor  GetLightColor() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cesium Streetlights")
	TArray<FTagValuesArray> Tags = {
		FTagValuesArray{
			TEXT("highway"),
			{
				TEXT("motorway"),
				TEXT("motorway_link"),
				TEXT("trunk"),
				TEXT("trunk_link"),
				TEXT("primary"),
				TEXT("primary_link"),
				TEXT("secondary"),
				TEXT("secondary_link"),
				TEXT("tertiary"),
				TEXT("residential"),
			},
		}
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cesium Streetlights", meta=(ClampMin="0.0", UIMin="0.0", Units="m"))
	double HeightAboveGround = 8.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cesium Streetlights", meta=(ClampMin="0.0", UIMin="0.0", Units="Lumens"))
	float Intensity = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cesium Streetlights", meta=(ClampMin="0.0", UIMin="0.0", Units="m"))
	double AdditionalSpace = 5.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cesium Streetlights", meta=(ClampMin="0.0", UIMin="0.0", Units="m"))
	float SourceRadius = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cesium Streetlights", meta=(ClampMin="0.0", UIMin="0.0", Units="m"))
	float AttenuationRadius = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cesium Streetlights")
	bool UseTemperatureForColor = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cesium Streetlights", meta=(EditCondition="UseTemperatureForColor", ClampMin="1000.0", UIMin="1000.0", Units="K"))
	float ColorTemperatureKelvins = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cesium Streetlights", meta=(EditCondition="!UseTemperatureForColor"))
	FColor Color = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cesium Streetlights")
	bool CastShadows = false;
};
