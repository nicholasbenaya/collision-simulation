#ifndef PARTICLE_HPP
#define PARTICLE_HPP

class Particle {
private:
  double x, y;   // Position
  double vx, vy; // Velocity
  double mass;   // Mass
public:
  Particle(double x, double y, double mass = 1.0);
  void setPosition(double newX, double newY);
  void setVelocity(double newVx, double newVy);
  void update(
      double dt, double windowWidth,
      double windowHeight); // Update position based on velocity and time step

  bool isColliding(const Particle &other) const;
  void resolveCollision(Particle &other);
  double getRadius() const; // Add this

  // getters
  double getX() const { return x; };
  double getY() const { return y; };
  double getVx() const { return vx; };
  double getVy() const { return vy; };
  double getMass() const { return mass; };
};

#endif