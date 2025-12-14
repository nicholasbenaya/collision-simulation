#ifndef QUAD_TREE_DETECTOR_HPP
#define QUAD_TREE_DETECTOR_HPP

#include "CollisionDetector.hpp"
#include <vector>

class Particle;

class QuadTreeDetector : public ICollisionDetector {
public:
  void detectAndResolve(std::vector<Particle> &particles) override;
};

#endif