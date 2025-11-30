#include "Particle.hpp"
#include <cmath>
#include <chrono>

Particle::Particle(double x, double y, double mass)
    : x(x), y(y), vx(100), vy(100), mass(mass), 
      creationTime(std::chrono::steady_clock::now()) {} // Set creation time

// ... rest of your existing Particle methods ...

bool Particle::isColliding(const Particle &other) const {
  double dx = x - other.x;
  double dy = y - other.y;
  double distance = sqrt(dx * dx + dy * dy);
  double minDistance = getRadius() + other.getRadius();
  return distance < minDistance;
}
double Particle::getRadius() const {
  return sqrt(mass) * 20; // Radius proportional to the square root of mass
}
void Particle::setVelocity(double newVx, double newVy) {
  vx = newVx;
  vy = newVy;
}
void Particle::resolveCollision(Particle &other) {
  // Calculate distance and overlap
  double dx = other.x - x;
  double dy = other.y - y;
  double distance = sqrt(dx * dx + dy * dy);

  if (distance == 0)
    return; // Avoid division by zero

  double minDistance = getRadius() + other.getRadius();
  double overlap = minDistance - distance;

  // Normalize direction vector
  double nx = dx / distance;
  double ny = dy / distance;

  // Separate particles
  double totalMass = mass + other.mass;
  double separation1 = overlap * (other.mass / totalMass);
  double separation2 = overlap * (mass / totalMass);

  x -= nx * separation1;
  y -= ny * separation1;
  other.x += nx * separation2;
  other.y += ny * separation2;

  // Calculate relative velocity
  double dvx = other.vx - vx;
  double dvy = other.vy - vy;

  // Calculate relative velocity along collision normal
  double dvn = dvx * nx + dvy * ny;

  // Don't resolve if velocities are separating
  if (dvn > 0)
    return;

  // Collision impulse (elastic collision)
  double impulse = 2 * dvn / totalMass;

  // Update velocities
  vx += impulse * other.mass * nx;
  vy += impulse * other.mass * ny;
  other.vx -= impulse * mass * nx;
  other.vy -= impulse * mass * ny;
}

void Particle::update(double dt, double windowWidth, double windowHeight) {
  x += vx * dt;
  y += vy * dt;

  double radius = sqrt(mass) * 20; // Calculate the radius once

  // Simple boundary collision
  if (x > windowWidth - radius) {
    x = windowWidth - radius;
    vx = -vx;
  } else if (x < radius) {
    x = radius;
    vx = -vx;
  }

  if (y > windowHeight - radius) {
    y = windowHeight - radius;
    vy = -vy;
  } else if (y < radius) { // Adjusted condition for the top edge
    y = radius;
    vy = -vy;
  }
}

void Particle::setPosition(double newX, double newY) {
  x = newX;
  y = newY;
}
