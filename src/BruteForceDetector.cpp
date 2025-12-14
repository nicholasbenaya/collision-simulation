#include "BruteForceDetector.hpp"
#include "Particle.hpp"

void BruteForceDetector::detectAndResolve(std::vector<Particle> &particles) {
  for (size_t i = 0; i < particles.size(); ++i) {
    for (size_t j = i + 1; j < particles.size(); ++j) {
      if (particles[i].isColliding(particles[j])) {
        particles[i].resolveCollision(particles[j]);
      }
    }
  }
}