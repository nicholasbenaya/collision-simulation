#include "QuadTreeDetector.hpp"
#include "Particle.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

namespace {

struct AABB {
  double x, y, w, h; // center (x,y), half-width = w/2, half-height = h/2
};

bool contains(const AABB &box, double px, double py) {
  return std::abs(px - box.x) <= box.w * 0.5 &&
         std::abs(py - box.y) <= box.h * 0.5;
}

bool intersects(const AABB &box, const AABB &range) {
  return std::abs(box.x - range.x) * 2 <= (box.w + range.w) &&
         std::abs(box.y - range.y) * 2 <= (box.h + range.h);
}

struct QuadNode {
  AABB boundary;
  std::vector<size_t> indices;
  std::unique_ptr<QuadNode> nw, ne, sw, se;
  bool divided = false;

  QuadNode(const AABB &b) : boundary(b) {}

  void subdivide() {
    double hw = boundary.w * 0.5;
    double hh = boundary.h * 0.5;
    double x = boundary.x;
    double y = boundary.y;
    nw = std::make_unique<QuadNode>(AABB{x - hw * 0.5, y - hh * 0.5, hw, hh});
    ne = std::make_unique<QuadNode>(AABB{x + hw * 0.5, y - hh * 0.5, hw, hh});
    sw = std::make_unique<QuadNode>(AABB{x - hw * 0.5, y + hh * 0.5, hw, hh});
    se = std::make_unique<QuadNode>(AABB{x + hw * 0.5, y + hh * 0.5, hw, hh});
    divided = true;
  }

  bool insert(size_t idx, const std::vector<Particle> &pts, size_t maxCap,
              size_t depth, size_t maxDepth) {
    const auto &p = pts[idx];
    double px = p.getX();
    double py = p.getY();
    if (!contains(boundary, px, py))
      return false;

    if (indices.size() < maxCap || depth >= maxDepth) {
      indices.push_back(idx);
      return true;
    }

    if (!divided)
      subdivide();

    return (nw->insert(idx, pts, maxCap, depth + 1, maxDepth) ||
            ne->insert(idx, pts, maxCap, depth + 1, maxDepth) ||
            sw->insert(idx, pts, maxCap, depth + 1, maxDepth) ||
            se->insert(idx, pts, maxCap, depth + 1, maxDepth));
  }

  void query(const AABB &range, std::vector<size_t> &out) const {
    if (!intersects(boundary, range))
      return;
    for (size_t i : indices) {
      out.push_back(i);
    }
    if (!divided)
      return;
    nw->query(range, out);
    ne->query(range, out);
    sw->query(range, out);
    se->query(range, out);
  }
};

} // namespace

void QuadTreeDetector::detectAndResolve(std::vector<Particle> &particles) {
  if (particles.size() < 2)
    return;

  // Hitung batas keseluruhan
  double minX = particles[0].getX(), maxX = particles[0].getX();
  double minY = particles[0].getY(), maxY = particles[0].getY();
  double maxR = particles[0].getRadius();
  for (const auto &p : particles) {
    minX = std::min(minX, p.getX());
    maxX = std::max(maxX, p.getX());
    minY = std::min(minY, p.getY());
    maxY = std::max(maxY, p.getY());
    maxR = std::max(maxR, p.getRadius());
  }

  // Sedikit padding supaya semua partikel masuk
  double pad = maxR;
  double width = (maxX - minX) + 2 * pad;
  double height = (maxY - minY) + 2 * pad;
  double cx = (minX + maxX) * 0.5;
  double cy = (minY + maxY) * 0.5;
  if (width == 0)
    width = 2 * maxR + 1.0;
  if (height == 0)
    height = 2 * maxR + 1.0;

  AABB world{cx, cy, width, height};
  QuadNode root(world);

  constexpr size_t MAX_CAP = 8;
  constexpr size_t MAX_DEPTH = 8;

  for (size_t i = 0; i < particles.size(); ++i) {
    root.insert(i, particles, MAX_CAP, 0, MAX_DEPTH);
  }

  // Untuk setiap partikel, query tetangga di sekitar radius + maxR
  std::vector<size_t> candidates;
  candidates.reserve(32);
  for (size_t i = 0; i < particles.size(); ++i) {
    const auto &p = particles[i];
    double r = p.getRadius();
    double rangeR = r + maxR;
    AABB range{p.getX(), p.getY(), rangeR * 2.0, rangeR * 2.0};
    candidates.clear();
    root.query(range, candidates);
    for (size_t j : candidates) {
      if (j <= i)
        continue; // hindari duplikasi
      if (p.isColliding(particles[j])) {
        // Resolusi dua arah
        particles[i].resolveCollision(particles[j]);
      }
    }
  }
}