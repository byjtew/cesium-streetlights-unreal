#pragma once

#include <unordered_set>
#include <numbers>

class SpacingGrid
{
public:
  explicit SpacingGrid(double spacing_degrees);

  void Reserve(size_t num_points);

  bool TryAdd(double latitude, double longitude);

protected:
  /// Computes the difference between two points in degrees.
  double GetDistance(double lat1, double lon1, double lat2, double lon2) const;

  static size_t GetHash(double latitude, double longitude);

private:
  const double m_spacing; // degrees
  std::unordered_set<size_t> m_pointsHashset; // Hash set to store unique points
};
