#ifndef COLLISION_DETECTOR_HPP
#define COLLISION_DETECTOR_HPP

#include <vector>
class Particle;

class ICollisionDetector {
public:
  virtual ~ICollisionDetector() = default;
  virtual void detectAndResolve(std::vector<Particle> &particles) = 0;
};

#endif