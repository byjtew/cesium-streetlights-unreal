#include "StreetLightGenerator.h"

#include <cassert>

#include "CesiumGeoreference.h"
#include <glm/detail/func_trigonometric.inl>
#include "internal/spacing_grid.h"
#include "internal/CesiumExtensions.h"
#include "OsmiumImports.h"
#include "internal/UnrealExtensions.h"

struct WayHandler : public osmium::handler::Handler
{
  explicit WayHandler(
    SpacingGrid spacingGrid,
    std::function<bool(double, double)> acceptCallback,
    std::function<bool(double, double)> placeCallback
  )
    : m_spacingGrid(std::move(spacingGrid)),
      m_acceptCallback(std::move(acceptCallback)),
      m_placeCallback(std::move(placeCallback))
  {
    assert(m_placeCallback);
  }

  void way(const osmium::Way& way) const
  {
    UE_LOG(LogTemp, Log, TEXT("Processing way %lld"), way.id());
    // if (m_stop) return;

    for (const auto& node : way.nodes())
    {
      if (not node.location().valid())
      {
        continue;
      }
      const double latitude = node.location().lat();
      const double longitude = node.location().lon();
      // UE_LOG(LogTemp, Log, TEXT("Processing way's node at (longitude=%f, latitude=%f)"), longitude, latitude);
      if (m_acceptCallback(latitude, longitude))
      {
        // if (m_spacingGrid.TryAdd(latitude, longitude))
        {
          // m_stop = m_placeCallback(latitude, longitude);
        }
      }
    }
  }

private:
  SpacingGrid m_spacingGrid;
  std::function<bool(double, double)> m_acceptCallback;
  std::function<bool(double, double)> m_placeCallback;
  bool m_stop = false;
};

AStreetLightGenerator::AStreetLightGenerator()
{
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.bStartWithTickEnabled = true;

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  RootComponent->SetMobility(EComponentMobility::Movable);
}

void AStreetLightGenerator::BeginPlay()
{
  Super::BeginPlay();
}

bool AStreetLightGenerator::ShouldTickIfViewportsOnly() const
{
  return true;
}

void AStreetLightGenerator::Tick(const float DeltaTime)
{
  Super::Tick(DeltaTime);

  const UWorld* world = GetWorld();
  const ACesiumGeoreference* georeference = ACesiumGeoreference::GetDefaultGeoreference(world);
  if (world && georeference)
  {
    auto [segments, thickness] = unreal_ext::DebugSphereDrawParametersBySize(SpawnConfiguration.SpawnRadiusMeters);
    DrawDebugSphere(
      world,
      GetActorLocation(),
      cesium_ext::MetersToUnreal(SpawnConfiguration.SpawnRadiusMeters, georeference),
      segments,
      FColor::Orange,
      false,
      -1,
      0,
      thickness
    );

    const FVector extent = FVector::OneVector * cesium_ext::MetersToUnreal(DebugDrawLightSizeMeters, georeference);
    for (auto* light : SpawnedStreetLights)
    {
      if (light && !light->IsValidLowLevelFast())
      {
        DrawDebugBox(
          world,
          light->GetActorLocation(),
          extent,
          FColor::Yellow
        );
      }
    }
  }
}

void AStreetLightGenerator::GenerateStreetLights()
{
  if (OsmFilepath.FilePath.IsEmpty())
  {
    UE_LOG(LogTemp, Error, TEXT("OsmFilepath is empty. Please set a valid OSM file path."));
    return;
  }
  if (not FPaths::FileExists(OsmFilepath.FilePath))
  {
    UE_LOG(LogTemp, Error, TEXT("Osm file '%s' does not exist."), *OsmFilepath.FilePath);
    return;
  }

  const std::string osm_file_path = TCHAR_TO_UTF8(*OsmFilepath.FilePath);

  const FVector actor_geolocation = ACesiumGeoreference::GetDefaultGeoreference(GetWorld())->TransformUnrealPositionToLongitudeLatitudeHeight(GetActorLocation());
  const double spacing_degrees = cesium_ext::MetersToEarthDegrees(actor_geolocation, SpawnConfiguration.SpaceBetweenLightsMeters.Get(35)).X;
  const SpacingGrid geogrid(cesium_ext::MetersToEarthDegrees(actor_geolocation, spacing_degrees).X);

  auto accept_callback = [&](const double latitude, const double longitude) -> bool
  {
    const bool within = cesium_ext::IsInRadius(latitude, longitude, actor_geolocation.Y, actor_geolocation.X, SpawnConfiguration.SpawnRadiusMeters);
    UE_LOG(LogTemp, Log, TEXT("Accepting location (longitude=%f, latitude=%f) to (longitude=%f, latitude=%f) within radius: %s"), actor_geolocation.X, actor_geolocation.Y,
           longitude, latitude, within ? TEXT("true") : TEXT("false"));
    return within;
  };

  auto place_callback = [&](const double latitude, const double longitude) -> bool
  {
    SpawnLightAtLocation(FVector(longitude, latitude, SpawnConfiguration.SpawnHeightMeters));
    return SpawnedStreetLights.Num() >= SpawnConfiguration.MaxNumberOfLights;
  };

  try
  {
    UE_LOG(LogTemp, Log, TEXT("Reading OSM file '%hs'..."), osm_file_path.c_str());
    {
      osmium::io::Reader reader(osm_file_path);
      osmium::apply(
        reader,
        WayHandler(
          (geogrid),
          (accept_callback),
          (place_callback)
        )
      );
      reader.close();
    }
    UE_LOG(LogTemp, Log, TEXT("Read OSM file '%hs' successfully."), osm_file_path.c_str());
  }
  catch (std::exception& e)
  {
    UE_LOG(LogTemp, Error, TEXT("Failed to read OSM file '%hs': %hs"), osm_file_path.c_str(), e.what());
  }
}

AActor* AStreetLightGenerator::SpawnLightAtLocation(const FVector& Location)
{
  if (not StreetLightActorClass)
  {
    UE_LOG(LogTemp, Error, TEXT("StreetLightActorClass is not set. Please set a valid street light actor class."));
    return nullptr; // Stop processing further ways
  }
  FVector location(Location.X, Location.Y, Location.Z);
  UE_LOG(LogTemp, Log, TEXT("Placing street light at (longitude=%f, latitude=%f, height=%f)"), Location.X, Location.Y, Location.Z);
  if (auto* georeference = ACesiumGeoreference::GetDefaultGeoreference(GetWorld()))
  {
    location = georeference->TransformLongitudeLatitudeHeightPositionToUnreal(location);
  }
  AActor* streetLight = GetWorld()->SpawnActor<AActor>(StreetLightActorClass, location, FRotator::ZeroRotator);
  if (streetLight)
  {
    // Set the light properties based on StyleConfiguration
    // This part depends on how your street light actor is set up
    // For example:
    // streetLight->SetColor(StyleConfiguration.Color);
    // streetLight->SetRange(StyleConfiguration.RangeMeters);
    // streetLight->SetIntensity(StyleConfiguration.Intensity);
    // streetLight->SetLightUnits(StyleConfiguration.LightUnits);
  }
  SpawnedStreetLights.Add(streetLight);
  return streetLight;
}
