#include "CesiumStreetlightsGeodeskDataSource.h"

#include <random>

#include "CesiumStreetlightsForUnreal.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"
#include "internal/CesiumExtensions.h"
#include "GeodeskImports.h"


namespace
{
	static void ParsePointsFromFeatures(const geodesk::Features& features, const std::function<void(FVector)>& for_each_position)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::ParsePointsFromFeatures);

		for (const geodesk::Feature& feature : features)
		{
			if (feature.isNode())
			{
				for_each_position(FVector(feature.lon(), feature.lat(), 0.0));
			}
			else if (feature.isWay())
			{
				for (const geodesk::Node& node : feature.nodes())
				{
					for_each_position(FVector(node.lon(), node.lat(), 0.0));
				}
			}
		}
	}

	FString GetGeodeskRequest(const TArray<FTagValuesArray>& Tags)
	{
		FString goql = "w";
		for (int32 i = 0; i < Tags.Num(); i++)
		{
			const FTagValuesArray& values_array = Tags[i];
			goql += "[" + values_array.Key;
			if (not values_array.Values.IsEmpty())
			{
				goql += "=";
				for (int32 j = 0; j < values_array.Values.Num(); j++)
				{
					const FString& v = values_array.Values[j];
					goql += v;

					if (j < values_array.Values.Num() - 1)
					{
						goql += ",";
					}
				}

				if (i < Tags.Num() - 1)
				{
					goql += ",";
				}
			}
			goql += "]";


			if (i < Tags.Num() - 1)
			{
				goql += ",";
			}
		}
		return goql;
	}
}


UCesiumStreetlightsGeodeskDataSource::UCesiumStreetlightsGeodeskDataSource()
{
	PrimaryComponentTick.bCanEverTick          = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCesiumStreetlightsGeodeskDataSource::RequestInternal(FCesiumStreetlightsRequestConfiguration configuration, TFunction<void(FCesiumStreetlightsRequestResult&&)> on_results)
{
	TArray<FVector> results = LoadGeoPositions(configuration);
	on_results(FCesiumStreetlightsRequestResult{.Geolocations = std::move(results)});
}

TArray<FVector> UCesiumStreetlightsGeodeskDataSource::LoadGeoPositions(const FCesiumStreetlightsRequestConfiguration& configuration) const
{
	TArray<FVector> geo_positions;

	if (GolFilepath.FilePath.IsEmpty())
	{
		UE_LOG(LogCesiumStreetlights, Warning, TEXT("No .gol file path specified."));
		return geo_positions;
	}

	if (not FPaths::FileExists(GolFilepath.FilePath))
	{
		UE_LOG(
			LogCesiumStreetlights,
			Warning,
			TEXT("The specified .gol file does not exist: %s"),
			*GolFilepath.FilePath
		);
		return geo_positions;
	}

	try
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::LoadGeoPositionsFromGolFile);

		const std::string gol_filepath_std = TCHAR_TO_UTF8(*GolFilepath.FilePath);
		geodesk::Features world(gol_filepath_std.c_str());

		const FString     goql_request     = GetGeodeskRequest(configuration.OsmTags);
		const std::string goql_request_std = TCHAR_TO_UTF8(*goql_request);
		{
			geo_positions.Emplace();

			UE_LOG(
				LogCesiumStreetlights,
				Log,
				TEXT("Processing Geodesk request: %s"),
				*goql_request
			);
			if (goql_request.IsEmpty()) { return geo_positions; }

			geodesk::Features features = world(goql_request_std.c_str());
			features                   = world.maxMetersFromLonLat(configuration.Radius, configuration.Origin.X, configuration.Origin.Y);

			UE_LOG(
				LogCesiumStreetlights,
				Log,
				TEXT("Found %llu features for request: %s"),
				features.count(),
				*goql_request
			);
			ParsePointsFromFeatures(
				features,
				[&](FVector position)
				{
					geo_positions.Add(std::move(position));
				}
			);
		}
	}
	catch (const std::exception& e)
	{
		UE_LOG(
			LogCesiumStreetlights,
			Error,
			TEXT("Exception while loading/parsing .gol file '%s': %s"),
			*GolFilepath.FilePath,
			*FString(e.what())
		);
	}

	return geo_positions;
}
