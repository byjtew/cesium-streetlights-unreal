#include "CesiumStreetlightsGeodeskVolumeActor.h"

#include <random>

#include "Cesium3DTileset.h"
#include "CesiumStreetlightComponent.h"
#include "CesiumStreetlightsForUnreal.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"
#include "internal/CesiumExtensions.h"
#include "GeodeskImports.h"

namespace
{
	struct [[maybe_unused]] FilterGeoPositionsByRelativeDistance
	{
		static void InPlace(TArray<FVector>& geo_positions, double min_distance_meters)
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::FilterGeoPositionsByRelativeDistance::InPlace);

			const double min_distance_meters_squared = min_distance_meters * min_distance_meters;

			TArray<int32> to_remove_indices;
			to_remove_indices.Reserve(geo_positions.Num() * 0.1); // arbitrary
			for (int i = 0; i < geo_positions.Num(); ++i)
			{
				const FVector& position = geo_positions[i];

				for (int32 j = i + 1; j < geo_positions.Num(); ++j)
				{
					const FVector& other_position = geo_positions[j];

					const double distance_meters         = cesium_ext::DistanceInMetersBetween(position, other_position);
					const double distance_meters_squared = distance_meters * distance_meters;

					if (distance_meters_squared > min_distance_meters_squared) [[likely]]
					{
						continue;
					}

					to_remove_indices.Add(i);
					break;
				}
			}
			for (int i = to_remove_indices.Num() - 1; i >= 0; --i)
			{
				geo_positions.RemoveAt(to_remove_indices[i], EAllowShrinking::No);
			}
			geo_positions.Shrink();
		}

		static TArray<FVector> OutOfPlace(const TArray<FVector>& geo_positions, double min_distance_meters)
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::FilterGeoPositionsByRelativeDistance::OutOfPlace);

			TArray<FVector> filtered_positions = geo_positions;
			InPlace(filtered_positions, min_distance_meters);
			return filtered_positions;
		}
	};

	struct [[maybe_unused]] FilterGeoPositionsByDistanceToOrigin
	{
		static void InPlace(TArray<FVector>& geo_positions, const FVector& origin, double max_distance_meters)
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::FilterGeoPositionsByDistanceToOrigin::InPlace);

			const double max_distance_meters_squared = max_distance_meters * max_distance_meters;

			geo_positions.RemoveAll([&](const FVector& position)
			{
				const double distance_meters         = cesium_ext::DistanceInMetersBetween(position, origin);
				const double distance_meters_squared = distance_meters * distance_meters;
				return distance_meters_squared > max_distance_meters_squared;
			});
			geo_positions.Shrink();
		}

		static TArray<FVector> OutOfPlace(const TArray<FVector>& geo_positions, const FVector& origin, double max_distance_meters)
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::FilterGeoPositionsByDistanceToOrigin::OutOfPlace);

			TArray<FVector> filtered_positions = geo_positions;
			InPlace(filtered_positions, origin, max_distance_meters);
			return filtered_positions;
		}
	};

	struct [[maybe_unused]] ShufflePositions
	{
		static void InPlace(TArray<FVector>& positions)
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::ShufflePositions::InPlace);

			std::shuffle(
				positions.GetData(),
				positions.GetData() + positions.Num(),
				std::default_random_engine{}
			);
		}

		static TArray<FVector> OutOfPlace(const TArray<FVector>& positions)
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::ShufflePositions::OutOfPlace);

			TArray<FVector> shuffled_positions = positions;
			InPlace(shuffled_positions);
			return shuffled_positions;
		}
	};

	struct [[maybe_unused]] JitterGeoPositions
	{
		static void InPlace(TArray<FVector>& geo_positions, double jitter_radius_meters)
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::JitterGeoPositions::InPlace);

			for (FVector& position : geo_positions)
			{
				position.X += FMath::RandRange(-jitter_radius_meters, jitter_radius_meters);
				position.Y += FMath::RandRange(-jitter_radius_meters, jitter_radius_meters);
			}
		}

		static TArray<FVector> OutOfPlace(const TArray<FVector>& geo_positions, double jitter_radius_meters)
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::JitterGeoPositions::OutOfPlace);

			TArray<FVector> jittered_positions = geo_positions;
			InPlace(jittered_positions, jitter_radius_meters);
			return jittered_positions;
		}
	};

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
}


ACesiumStreetlightsGeodeskVolumeActor::ACesiumStreetlightsGeodeskVolumeActor()
{
#if WITH_EDITOR
	PrimaryActorTick.bCanEverTick = true;
#else
	PrimaryActorTick.bCanEverTick = false;
#endif

	PrimaryActorTick.bStartWithTickEnabled = true;

	RootComponent = Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ACesiumStreetlightsGeodeskVolumeActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACesiumStreetlightsGeodeskVolumeActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if WITH_EDITOR
	UWorld*              world        = GetWorld();
	ACesiumGeoreference* georeference = ACesiumGeoreference::GetDefaultGeoreference(world);
	if (world && world->WorldType == EWorldType::Editor && georeference)
	{
		DrawDebugSphere(
			world,
			GetActorLocation(),
			cesium_ext::MetersToUnreal(Radius, georeference),
			32,
			FColor::Orange,
			false,
			-1.0f,
			0,
			cesium_ext::MetersToUnreal(1.0, georeference)
		);
	}
#endif
}

void ACesiumStreetlightsGeodeskVolumeActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Reset();

	Super::EndPlay(EndPlayReason);
}

void ACesiumStreetlightsGeodeskVolumeActor::Generate()
{
	Reset();

	if (not GroundTileset)
	{
		UE_LOG(
			LogCesiumStreetlights,
			Warning,
			TEXT("No GroundTileset assigned to CesiumStreetlightsGeodeskVolumeComponent on actor %s"),
			*GetName()
		);
		return;
	}

	if (StyleConfigurations.IsEmpty())
	{
		UE_LOG(
			LogCesiumStreetlights,
			Warning,
			TEXT("No StyleConfigurations assigned to CesiumStreetlightsGeodeskVolumeComponent on actor %s"),
			*GetName()
		);
		return;
	}

	UWorld* world = GetWorld();
	if (not world)
	{
		UE_LOG(
			LogCesiumStreetlights,
			Warning,
			TEXT("No World found for CesiumStreetlightsGeodeskVolumeComponent")
		);
		return;
	}

	ACesiumGeoreference* georeference = ACesiumGeoreference::GetDefaultGeoreference(world);
	if (not georeference)
	{
		UE_LOG(
			LogCesiumStreetlights,
			Warning,
			TEXT("No CesiumGeoreference found in World for CesiumStreetlightsGeodeskVolumeComponent")
		);
		return;
	}

	const FVector geo_origin = georeference->TransformUnrealPositionToLongitudeLatitudeHeight(GetActorLocation());

	auto generate_lambda = [self = this, world, georeference, geo_origin]()
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::Generate);

		FGeodeskRequestsConfiguration requests_config;
		{
			requests_config.GolFilepath     = self->GolFilepath.FilePath;
			requests_config.OriginAndRadius = {FVector2d(geo_origin.X, geo_origin.Y), self->Radius};
			for (const auto& style : self->StyleConfigurations)
			{
				requests_config.GoqlRequests.Add(style.GetGeodeskRequest());
			}
		}

		TArray<TArray<FVector>> geopositions_by_style = LoadGeoPositionsFromGolFile(requests_config);
		for (int16 i = 0; i < self->StyleConfigurations.Num(); ++i)
		{
			if (not geopositions_by_style.IsValidIndex(i)) { continue; }

			const FStreetlightStyleConfiguration& style        = self->StyleConfigurations[i];
			TArray<FVector>&                      geopositions = geopositions_by_style[i];
			{
				FilterGeoPositionsByDistanceToOrigin::InPlace(geopositions, geo_origin, self->Radius);
			}
			{
				const double spacing = style.AttenuationRadius + style.AdditionalSpace / 2;
				FilterGeoPositionsByRelativeDistance::InPlace(geopositions, spacing);
			}
			{
				ShufflePositions::InPlace(geopositions);
			}
			auto on_geoposition_heights_sampled_lambda = [self, georeference, style](
				ACesium3DTileset*,
				const TArray<FCesiumSampleHeightResult>& results,
				const TArray<FString>&
			)
			{
				TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::Generate::OnGeopositionHeightsSampled);

				TArray<FVector> ue_positions;
				{
					ue_positions.Reserve(results.Num());
					for (const FCesiumSampleHeightResult& result : results)
					{
						if (result.SampleSuccess)
						{
							ue_positions.Add(georeference->TransformLongitudeLatitudeHeightPositionToUnreal(result.LongitudeLatitudeHeight));
						}
					}
				}
				auto spawn_streetlights_lambda = [georeference](
					ACesiumStreetlightsGeodeskVolumeActor* self,
					const FStreetlightStyleConfiguration&  style,
					const TArray<FVector>&&                geopositions
				)
				{
					TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::Generate::SpawnStreetlightComponents);

					// Spawn streetlights
					UE_LOG(
						LogCesiumStreetlights,
						Log,
						TEXT("Spawning %d streetlights"),
						geopositions.Num()
					);

					for (const FVector& ue_position : geopositions)
					{
						UCesiumStreetlightComponent* light = self->SpawnStreetlightComponent();
						if (not light)
						{
							UE_LOG(
								LogCesiumStreetlights,
								Warning,
								TEXT("Failed to spawn streetlight component")
							);
							continue;
						}
						light->ApplyStyle(style, georeference);
						light->SetWorldLocation(ue_position, false, nullptr, ETeleportType::TeleportPhysics);
						light->SetMobility(EComponentMobility::Static);
						self->SpawnedStreetlights.Add(light);
					}
				};
				AsyncTask(
					ENamedThreads::Type::GameThread,
					[spawn_streetlights_lambda, self, style, ue_positions = std::move(ue_positions)]()
					{
						spawn_streetlights_lambda(self, style, std::move(ue_positions));
					}
				);
			};

			check(self->GroundTileset);
			self->GroundTileset->SampleHeightMostDetailed(
				std::move(geopositions),
				FCesiumSampleHeightMostDetailedCallback::CreateLambda(on_geoposition_heights_sampled_lambda)
			);
		}

		self->Modify();
	};

	AsyncTask(
		ENamedThreads::Type::AnyThread,
		generate_lambda
	);
}

void ACesiumStreetlightsGeodeskVolumeActor::DryRun()
{
	if (not GroundTileset)
	{
		UE_LOG(
			LogCesiumStreetlights,
			Warning,
			TEXT("No GroundTileset assigned to CesiumStreetlightsGeodeskVolumeComponent on actor %s"),
			*GetName()
		);
		return;
	}

	if (StyleConfigurations.IsEmpty())
	{
		UE_LOG(
			LogCesiumStreetlights,
			Warning,
			TEXT("No StyleConfigurations assigned to CesiumStreetlightsGeodeskVolumeComponent on actor %s"),
			*GetName()
		);
		return;
	}

	UWorld* world = GetWorld();
	if (not world)
	{
		UE_LOG(
			LogCesiumStreetlights,
			Warning,
			TEXT("No World found for CesiumStreetlightsGeodeskVolumeComponent")
		);
		return;
	}

	ACesiumGeoreference* georeference = ACesiumGeoreference::GetDefaultGeoreference(world);
	if (not georeference)
	{
		UE_LOG(
			LogCesiumStreetlights,
			Warning,
			TEXT("No CesiumGeoreference found in World for CesiumStreetlightsGeodeskVolumeComponent")
		);
		return;
	}

	const FVector geo_origin = georeference->TransformUnrealPositionToLongitudeLatitudeHeight(GetActorLocation());

	auto generate_lambda = [self = this, world, georeference, geo_origin]()
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::Generate);

		FGeodeskRequestsConfiguration requests_config;
		{
			requests_config.GolFilepath     = self->GolFilepath.FilePath;
			requests_config.OriginAndRadius = {FVector2d(geo_origin.X, geo_origin.Y), self->Radius};
			for (const auto& style : self->StyleConfigurations)
			{
				requests_config.GoqlRequests.Add(style.GetGeodeskRequest());
			}
		}

		TArray<TArray<FVector>> geopositions_by_style = LoadGeoPositionsFromGolFile(requests_config);
		for (int16 i = 0; i < self->StyleConfigurations.Num(); ++i)
		{
			if (not geopositions_by_style.IsValidIndex(i)) { continue; }

			const FStreetlightStyleConfiguration& style        = self->StyleConfigurations[i];
			TArray<FVector>&                      geopositions = geopositions_by_style[i];
			{
				FilterGeoPositionsByDistanceToOrigin::InPlace(geopositions, geo_origin, self->Radius);
			}
			{
				const double spacing = style.AttenuationRadius + style.AdditionalSpace / 2;
				FilterGeoPositionsByRelativeDistance::InPlace(geopositions, spacing);
			}
			{
				ShufflePositions::InPlace(geopositions);
			}
			{
				// @todo: Sample altitude from CesiumTileset3D

				// @temporary: altitude based on actor's
				{
					const double actor_altitude_meters = geo_origin.Z;
					for (FVector& position : geopositions)
					{
						position.Z = actor_altitude_meters + style.HeightAboveGround;
					}
				}
			}
			{
				// @temporary: Convert geo-positions to unreal-positions
				{
					for (FVector& position : geopositions)
					{
						position = georeference->TransformLongitudeLatitudeHeightPositionToUnreal(position);
					}
				}
			}
			{
				// Spawn streetlights

				UE_LOG(
					LogCesiumStreetlights,
					Log,
					TEXT("Would spawn %d streetlights for style %d"),
					geopositions.Num(),
					i
				);
			}
		}

		self->Modify();
	};

	AsyncTask(
		ENamedThreads::Type::AnyThread,
		generate_lambda
	);
}

void ACesiumStreetlightsGeodeskVolumeActor::Reset()
{
	for (UCesiumStreetlightComponent* light : SpawnedStreetlights)
	{
		if (light)
		{
			light->DestroyComponent();
		}
		light = nullptr;
	}
	SpawnedStreetlights.Empty();

	Modify();
}

void ACesiumStreetlightsGeodeskVolumeActor::UpdateCurrentLightsStyle()
{
	if (not StyleConfigurations.IsEmpty())
	{
		for (UCesiumStreetlightComponent* light : SpawnedStreetlights)
		{
			if (light)
			{
				UE_LOG(
					LogCesiumStreetlights,
					Log,
					TEXT("Updating style of streetlight component %s"),
					*light->GetName()
				);
				light->ApplyStyle(StyleConfigurations[0], ACesiumGeoreference::GetDefaultGeoreference(GetWorld()));
			}
		}
	}
	Modify();
}

UCesiumStreetlightComponent* ACesiumStreetlightsGeodeskVolumeActor::SpawnStreetlightComponent() const
{
	UCesiumStreetlightComponent* streetlight_component = nullptr;

	streetlight_component = NewObject<UCesiumStreetlightComponent>(GetRootComponent());
	if (streetlight_component)
	{
		streetlight_component->RegisterComponent();
		streetlight_component->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}

	return streetlight_component;
}

TArray<TArray<FVector>> ACesiumStreetlightsGeodeskVolumeActor::LoadGeoPositionsFromGolFile(FGeodeskRequestsConfiguration requests)
{
	TArray<TArray<FVector>> geo_positions;

	if (requests.GolFilepath.IsEmpty())
	{
		UE_LOG(LogCesiumStreetlights, Warning, TEXT("No .gol file path specified."));
		return geo_positions;
	}

	if (not FPaths::FileExists(requests.GolFilepath))
	{
		UE_LOG(
			LogCesiumStreetlights,
			Warning,
			TEXT("The specified .gol file does not exist: %s"),
			*requests.GolFilepath
		);
		return geo_positions;
	}

	try
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskVolumeComponent::LoadGeoPositionsFromGolFile);

		const std::string gol_filepath_std = TCHAR_TO_UTF8(*requests.GolFilepath);
		geodesk::Features world(gol_filepath_std.c_str());

		geo_positions.Reserve(requests.GoqlRequests.Num());
		for (const FString& request : requests.GoqlRequests)
		{
			geo_positions.Emplace();

			UE_LOG(
				LogCesiumStreetlights,
				Log,
				TEXT("Processing Geodesk request: %s"),
				*request
			);
			if (request.IsEmpty()) { continue; }

			const std::string request_std = TCHAR_TO_UTF8(*request);
			geodesk::Features features    = world(request_std.c_str());
			if (requests.OriginAndRadius.IsSet())
			{
				const auto [origin, radius] = requests.OriginAndRadius.GetValue();
				features                    = world.maxMetersFromLonLat(radius, origin.X, origin.Y);
			}
			UE_LOG(
				LogCesiumStreetlights,
				Log,
				TEXT("Found %llu features for request: %s"),
				features.count(),
				*request
			);
			ParsePointsFromFeatures(
				features,
				[&](FVector position)
				{
					geo_positions.Last().Add(std::move(position));
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
			*requests.GolFilepath,
			*FString(e.what())
		);
	}

	return geo_positions;
}
