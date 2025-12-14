#ifndef BRUTE_FORCE_DETECTOR_HPP
#define BRUTE_FORCE_DETECTOR_HPP

#include "CollisionDetector.hpp"
#include <vector>

class Particle;

class BruteForceDetector : public ICollisionDetector {
public:
  void detectAndResolve(std::vector<Particle> &particles) override;
};

#endif