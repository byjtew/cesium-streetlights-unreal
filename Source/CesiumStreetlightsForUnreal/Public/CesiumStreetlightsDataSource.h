#pragma once

#include "CoreMinimal.h"
#include "StreetlightStyleConfiguration.h"
#include "CesiumStreetlightsDataSource.generated.h"


USTRUCT(BlueprintType)
struct FCesiumStreetlightsRequestConfiguration
{
	GENERATED_BODY()

	/**
	 *	[
	 *		[ "highway", ["motorway", "residential", ...] ],
	 *		...
	 *	]
	 */
	TArray<FTagValuesArray> OsmTags;

	/**
	 * The origin point [longitude,latitude] (in degrees)
	 */
	FVector2d Origin;

	/**
	 * The radius (in meters) of the area.
	 */
	double Radius;
};

USTRUCT(BlueprintType)
struct FCesiumStreetlightsRequestResult
{
	GENERATED_BODY()

	/**
	 * Array of geo-locations of streetlights [longitude, latitude, height in meters]
	 */
	TArray<FVector> Geolocations;
};

/**
 *
 */
UCLASS(Abstract, Hidden)
class CESIUMSTREETLIGHTSFORUNREAL_API UCesiumStreetlightsDataSource : public UActorComponent
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintCallable)
	void AsyncRequest(ENamedThreads::Type thread_name, FCesiumStreetlightsRequestConfiguration configuration, TFunction<void(FCesiumStreetlightsRequestResult&&)> on_results);

	// UFUNCTION(BlueprintCallable)
	void Request(FCesiumStreetlightsRequestConfiguration configuration, TFunction<void(FCesiumStreetlightsRequestResult&&)> on_results);

protected:
	virtual void RequestInternal(FCesiumStreetlightsRequestConfiguration configuration, TFunction<void(FCesiumStreetlightsRequestResult&&)> on_results);;
};
