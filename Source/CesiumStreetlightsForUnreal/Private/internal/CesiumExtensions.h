#pragma once

#include <numbers>
#include <glm/trigonometric.hpp>

#include "CoreMinimal.h"
#include "CesiumGeoreference.h"

namespace cesium_ext
{
	static constexpr double EarthRadius = 6372797.560856; // meters

	template <typename T>
	static constexpr T FORCEINLINE MetersToUnreal(T meters, double cesium_scale = 100.0)
	{
		return meters * cesium_scale;
	}

	template <typename T>
	static constexpr T FORCEINLINE UnrealToMeters(T unreal, double cesium_scale = 100.0)
	{
		return unreal / cesium_scale;
	}

	template <typename T>
	static T FORCEINLINE MetersToUnreal(T meters, const ACesiumGeoreference* georeference)
	{
		if (georeference)
		{
			return MetersToUnreal(meters, georeference->GetScale());
		}
		return MetersToUnreal(meters);
	}

	template <typename T>
	static T FORCEINLINE UnrealToMeters(T unreal, const ACesiumGeoreference* georeference)
	{
		if (georeference)
		{
			return UnrealToMeters(unreal, georeference->GetScale());
		}
		return UnrealToMeters(unreal);
	}

	/**
	 * Converts meters to Earth degrees based on the given geolocation.
	 * @param geolocation (longitude, latitude, height)
	 * @param meters
	 * @return FVector2d representing the Earth degrees (longitude, latitude) corresponding to the given meters.
	 */
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
		constexpr double R            = 6371e3; // Earth's radius in meters
		const double     phi1         = a_latitude * std::numbers::pi / 180.0;
		const double     phi2         = b_latitude * std::numbers::pi / 180.0;
		const double     delta_phi    = (b_latitude - a_latitude) * std::numbers::pi / 180.0;
		const double     delta_lambda = (b_longitude - a_longitude) * std::numbers::pi / 180.0;

		const double a = std::sin(delta_phi / 2) * std::sin(delta_phi / 2) + std::cos(phi1) * std::cos(phi2) * std::sin(delta_lambda / 2) * std::sin(delta_lambda / 2);
		const double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

		return R * c; // Distance in meters
	}

	/**
	* Calculates the distance in meters between two geolocations.
	* @param a_geolocation (longitude, latitude, height)
	* @param b_geolocation (longitude, latitude, height)
	* @return Distance in meters between the two geolocations.
	*/
	static double DistanceInMetersBetween(const FVector& a_geolocation, const FVector& b_geolocation)
	{
		return DistanceInMetersBetween(a_geolocation.Y, a_geolocation.X, b_geolocation.Y, b_geolocation.X);
	}

	static bool IsInRadius(double pt_latitude, double pt_longitude, double origin_latitude, double origin_longitude, double radius_meters)
	{
		return DistanceInMetersBetween(pt_latitude, pt_longitude, origin_latitude, origin_longitude) <= radius_meters;
	}
};
