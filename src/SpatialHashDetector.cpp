#include "SpatialHashDetector.hpp"
#include "Particle.hpp"
#include <cmath>

namespace {
// Helper to pack 2D cell coords into a single key
inline long long packKey(int cx, int cy) {
  return (static_cast<long long>(cx) << 32) ^ (static_cast<unsigned int>(cy));
}
} // namespace

SpatialHashDetector::SpatialHashDetector(double cellSize)
    : cellSize(cellSize) {}

SpatialHashDetector::CellKey SpatialHashDetector::keyFor(double x,
                                                         double y) const {
  int cx = static_cast<int>(std::floor(x / cellSize));
  int cy = static_cast<int>(std::floor(y / cellSize));
  return packKey(cx, cy);
}

void SpatialHashDetector::detectAndResolve(std::vector<Particle> &particles) {
  grid.clear();
  grid.reserve(particles.size());

  // Insert particles into grid cells
  for (size_t i = 0; i < particles.size(); ++i) {
    auto &p = particles[i];
    CellKey k = keyFor(p.getX(), p.getY());
    grid[k].push_back(i);
  }

  // Neighbor cell offsets (including current cell)
  const int offsets[9][2] = {{0, 0}, {1, 0},  {-1, 0}, {0, 1},  {0, -1},
                             {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

  // For each cell, check collisions with indices in the cell and neighbor cells
  for (const auto &entry : grid) {
    const auto &indices = entry.second;
    // Intra-cell checks
    for (size_t a = 0; a < indices.size(); ++a) {
      size_t i = indices[a];
      Particle &pi = particles[i];

      // Same cell pairs
      for (size_t b = a + 1; b < indices.size(); ++b) {
        size_t j = indices[b];
        if (pi.isColliding(particles[j])) {
          pi.resolveCollision(particles[j]);
        }
      }

      // Neighbor cells
      // Compute current cell coords once
      int cx = static_cast<int>(std::floor(pi.getX() / cellSize));
      int cy = static_cast<int>(std::floor(pi.getY() / cellSize));
      for (const auto &off : offsets) {
        int nx = cx + off[0];
        int ny = cy + off[1];
        CellKey nk = packKey(nx, ny);
        auto it = grid.find(nk);
        if (it == grid.end())
          continue;
        const auto &nidx = it->second;
        for (size_t k = 0; k < nidx.size(); ++k) {
          size_t j = nidx[k];
          if (j <= i)
            continue; // avoid double checking
          if (pi.isColliding(particles[j])) {
            pi.resolveCollision(particles[j]);
          }
        }
      }
    }
  }
}