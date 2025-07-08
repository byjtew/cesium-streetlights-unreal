#include "spacing_grid.h"

SpacingGrid::SpacingGrid(double spacing_degrees)
  : m_spacing(spacing_degrees)
{
}

void SpacingGrid::Reserve(size_t num_points)
{
  m_pointsHashset.reserve(num_points);
}

bool SpacingGrid::TryAdd(double latitude, double longitude)
{
  // Round the latitude and longitude to the nearest grid point based on the spacing
  latitude = std::round(latitude / m_spacing) * m_spacing;
  longitude = std::round(longitude / m_spacing) * m_spacing;
  // Check if the point is already in the hash set
  return m_pointsHashset.insert(GetHash(latitude, longitude)).second;
}

double SpacingGrid::GetDistance(double lat1, double lon1, double lat2, double lon2) const
{
  // Haversine formula to calculate distance between two points on the Earth
  constexpr double R = 6371e3; // Earth's radius in meters
  const double phi1 = lat1 * std::numbers::pi / 180.0;
  const double phi2 = lat2 * std::numbers::pi / 180.0;
  const double delta_phi = (lat2 - lat1) * std::numbers::pi / 180.0;
  const double delta_lambda = (lon2 - lon1) * std::numbers::pi / 180.0;

  const double a = sin(delta_phi / 2) * sin(delta_phi / 2) +
    cos(phi1) * cos(phi2) *
    sin(delta_lambda / 2) * sin(delta_lambda / 2);
  const double c = 2 * atan2(sqrt(a), sqrt(1 - a));

  return R * c; // Distance in meters
}

size_t SpacingGrid::GetHash(double latitude, double longitude)
{
  // Use a simple hash function based on the latitude and longitude
  return std::hash<double>()(latitude) ^ std::hash<double>()(longitude);
}
