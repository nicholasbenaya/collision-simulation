#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "CollisionDetector.hpp"
#include "Particle.hpp"

class Simulation {
public:
  Simulation(sf::RenderWindow &window);

  void generateParticles(int count);
  void addParticles(int count);
  void removeOldest(int count);
  void update(double dt);
  void render();

  void setDetector(std::unique_ptr<ICollisionDetector> detector,
                   const std::string &name);
  const char *currentAlgorithmName() const;

  int particleCount() const { return static_cast<int>(particles.size()); }

private:
  sf::RenderWindow &window;
  std::vector<Particle> particles;
  std::vector<sf::CircleShape> shapes;
  std::unique_ptr<ICollisionDetector> detector;
  std::string algoName;

  void clampParticleToWindow(Particle &particle);
  std::vector<Particle> generateParticlesInternal(int count);
  std::vector<sf::CircleShape> createShapes(const std::vector<Particle> &pts);
};

#endif