#ifndef SPATIAL_HASH_DETECTOR_HPP
#define SPATIAL_HASH_DETECTOR_HPP

#include "CollisionDetector.hpp"
#include <unordered_map>
#include <vector>

class Particle;

class SpatialHashDetector : public ICollisionDetector {
public:
  explicit SpatialHashDetector(double cellSize = 40.0);
  void detectAndResolve(std::vector<Particle> &particles) override;

private:
  double cellSize;
  using CellKey = long long;
  std::unordered_map<CellKey, std::vector<size_t>> grid;

  CellKey keyFor(double x, double y) const;
};

#endif