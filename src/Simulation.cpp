#include "Simulation.hpp"
#include <algorithm>
#include <cmath>
#include <random>

Simulation::Simulation(sf::RenderWindow &window) : window(window) {}

void Simulation::setDetector(std::unique_ptr<ICollisionDetector> d,
                             const std::string &name) {
  detector = std::move(d);
  algoName = name;
}

const char *Simulation::currentAlgorithmName() const {
  return algoName.c_str();
}

std::vector<Particle> Simulation::generateParticlesInternal(int count) {
  std::vector<Particle> pts;

  std::random_device rd;
  std::mt19937 gen(rd());
  double windowWidth = window.getSize().x;
  double windowHeight = window.getSize().y;
  double windowArea = windowWidth * windowHeight;
  double maxTotalParticleArea = windowArea * 0.3;
  double averageParticleArea = maxTotalParticleArea / count;
  double averageRadius = std::sqrt(averageParticleArea / M_PI);
  double maxMass = std::pow(averageRadius / 20.0, 2);
  double minMass = maxMass * 0.1;
  if (maxMass < 0.1)
    maxMass = 0.1;
  if (minMass < 0.01)
    minMass = 0.01;
  if (maxMass > 10.0)
    maxMass = 10.0;
  std::uniform_real_distribution<double> massDist(minMass, maxMass);
  std::uniform_real_distribution<double> velocityDist(-100.0, 100.0);

  for (int i = 0; i < count; ++i) {
    double mass = massDist(gen);
    double radius = std::sqrt(mass) * 20;
    double minX = radius;
    double maxX = windowWidth - radius;
    double minY = radius;
    double maxY = windowHeight - radius;
    if (minX >= maxX || minY >= maxY) {
      mass = std::min(mass, 0.01);
      radius = std::sqrt(mass) * 20;
      minX = radius;
      maxX = windowWidth - radius;
      minY = radius;
      maxY = windowHeight - radius;
    }
    std::uniform_real_distribution<double> xDist(minX,
                                                 std::max(minX + 1.0, maxX));
    std::uniform_real_distribution<double> yDist(minY,
                                                 std::max(minY + 1.0, maxY));
    double x = xDist(gen);
    double y = yDist(gen);
    Particle p(x, y, mass);
    p.setVelocity(velocityDist(gen), velocityDist(gen));
    pts.push_back(p);
  }
  return pts;
}

std::vector<sf::CircleShape>
Simulation::createShapes(const std::vector<Particle> &pts) {
  std::vector<sf::CircleShape> s;
  for (size_t i = 0; i < pts.size(); ++i) {
    const Particle &p = pts[i];
    sf::CircleShape shape(p.getRadius());
    sf::Uint8 red = static_cast<sf::Uint8>((i * 50) % 255);
    sf::Uint8 green = static_cast<sf::Uint8>((i * 100) % 255);
    sf::Uint8 blue = static_cast<sf::Uint8>((i * 150) % 255);
    shape.setFillColor(sf::Color(red, green, blue));
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(p.getX(), p.getY());
    s.push_back(shape);
  }
  return s;
}

void Simulation::generateParticles(int count) {
  particles = generateParticlesInternal(count);
  shapes = createShapes(particles);
}

void Simulation::addParticles(int count) {
  auto newPts = generateParticlesInternal(count);
  auto newShapes = createShapes(newPts);
  particles.insert(particles.end(), newPts.begin(), newPts.end());
  shapes.insert(shapes.end(), newShapes.begin(), newShapes.end());
}

void Simulation::removeOldest(int count) {
  int removeCount = std::min(count, static_cast<int>(particles.size()));
  std::vector<std::pair<std::chrono::steady_clock::time_point, size_t>> timeIdx;
  timeIdx.reserve(particles.size());
  for (size_t i = 0; i < particles.size(); ++i) {
    timeIdx.push_back({particles[i].getCreationTime(), i});
  }
  std::sort(timeIdx.begin(), timeIdx.end());
  std::vector<size_t> indices;
  indices.reserve(removeCount);
  for (int i = 0; i < removeCount; ++i)
    indices.push_back(timeIdx[i].second);
  std::sort(indices.rbegin(), indices.rend());
  for (size_t idx : indices) {
    particles.erase(particles.begin() + idx);
    shapes.erase(shapes.begin() + idx);
  }
}

void Simulation::clampParticleToWindow(Particle &particle) {
  double radius = particle.getRadius();
  double x = particle.getX();
  double y = particle.getY();
  double ww = window.getSize().x;
  double wh = window.getSize().y;
  if (x < radius)
    particle.setPosition(radius, y);
  else if (x > ww - radius)
    particle.setPosition(ww - radius, y);
  if (y < radius)
    particle.setPosition(particle.getX(), radius);
  else if (y > wh - radius)
    particle.setPosition(particle.getX(), wh - radius);
}

void Simulation::update(double dt) {
  auto ws = window.getSize();
  for (auto &particle : particles) {
    particle.update(dt, ws.x, ws.y);
  }
  if (detector) {
    detector->detectAndResolve(particles);
  }
  for (size_t i = 0; i < particles.size(); ++i) {
    shapes[i].setPosition(particles[i].getX(), particles[i].getY());
  }
}

void Simulation::render() {
  for (const auto &shape : shapes)
    window.draw(shape);
}