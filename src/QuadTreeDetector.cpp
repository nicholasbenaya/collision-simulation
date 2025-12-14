#include "QuadTreeDetector.hpp"
#include "Particle.hpp"

void QuadTreeDetector::detectAndResolve(std::vector<Particle> &particles) {
  // Placeholder: same as brute force for now
  for (size_t i = 0; i < particles.size(); ++i) {
    for (size_t j = i + 1; j < particles.size(); ++j) {
      if (particles[i].isColliding(particles[j])) {
        particles[i].resolveCollision(particles[j]);
      }
    }
  }
}