#include "Particle.hpp"
#include <cmath>
Particle::Particle(double x, double y, double mass) : x(x), y(y), vx(100), vy(100), mass(mass) {}

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