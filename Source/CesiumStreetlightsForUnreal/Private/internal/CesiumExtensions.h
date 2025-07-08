#pragma once

#include <numbers>
#include "CoreMinimal.h"
#include "CesiumGeoreference.h"

namespace cesium_ext
{
  static constexpr double EarthRadius = 6372797.560856; // meters

  static constexpr double FORCEINLINE MetersToUnreal(double meters, double cesium_scale = 100.0)
  {
    return meters * cesium_scale;
  }

  static constexpr double FORCEINLINE UnrealToMeters(double unreal, double cesium_scale = 100.0)
  {
    return unreal / std::max(std::numeric_limits<double>::epsilon(), cesium_scale);
  }

  static double FORCEINLINE MetersToUnreal(double meters, const ACesiumGeoreference* georeference)
  {
    if (georeference)
    {
      return MetersToUnreal(meters, georeference->GetScale());
    }
    return MetersToUnreal(meters);
  }

  static double FORCEINLINE UnrealToMeters(double unreal, const ACesiumGeoreference* georeference)
  {
    if (georeference)
    {
      return UnrealToMeters(unreal, georeference->GetScale());
    }
    return UnrealToMeters(unreal);
  }

  static FVector2d FORCEINLINE MetersToEarthDegrees(const FVector& geolocation, double meters)
  {
    return {
      glm::degrees(meters / (EarthRadius * std::cos(glm::radians(geolocation.Y)))),
      glm::degrees(meters / EarthRadius)
    };
  }

  static double DistanceInMetersBetween(double a_latitude, double a_longitude, double b_latitude, double b_longitude)
  {
    // Haversine formula to calculate distance between two points on the Earth
    constexpr double R = 6371e3; // Earth's radius in meters
    const double phi1 = a_latitude * std::numbers::pi / 180.0;
    const double phi2 = b_latitude * std::numbers::pi / 180.0;
    const double delta_phi = (b_latitude - a_latitude) * std::numbers::pi / 180.0;
    const double delta_lambda = (b_longitude - a_longitude) * std::numbers::pi / 180.0;

    const double a = std::sin(delta_phi / 2) * std::sin(delta_phi / 2) + std::cos(phi1) * std::cos(phi2) * std::sin(delta_lambda / 2) * std::sin(delta_lambda / 2);
    const double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    return R * c; // Distance in meters
  }

  static bool IsInRadius(double pt_latitude, double pt_longitude, double origin_latitude, double origin_longitude, double radius_meters)
  {
    return DistanceInMetersBetween(pt_latitude, pt_longitude, origin_latitude, origin_longitude) <= radius_meters;
  }
};
