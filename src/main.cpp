#include "Particle.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

#include <cmath>

using namespace std;
int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Collision Sim");

  // Particle object creation
  Particle p1(0, 0, 5.0);
  Particle p2(200, 200, 10.0);

  sf::CircleShape shape1(sqrt(p1.getMass()) *
                         20); // Radius proportional to mass
  shape1.setFillColor(sf::Color::Red);

  shape1.setPosition(p1.getX(), p1.getY());

  sf::CircleShape shape2(sqrt(p2.getMass()) *
                         20); // Radius proportional to mass
  shape2.setFillColor(sf::Color::Blue);
  shape2.setPosition(p2.getX(), p2.getY());

  sf::Clock clock;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    double dt = clock.restart().asSeconds();
    if (dt > 0.5)
      dt = 0.5; // clamp to avoid huge jumps
    
    // p1.update(1/60.0, window.getSize().x, window.getSize().y);
    p1.update(dt, window.getSize().x, window.getSize().y);
    shape1.setPosition(p1.getX(), p1.getY());

    // p2.update(1/60.0, window.getSize().x, window.getSize().y);
    p2.update(dt, window.getSize().x, window.getSize().y);
    shape2.setPosition(p2.getX(), p2.getY());

    std::cout << "Particle 1 Position: (" << p1.getX() << ", " << p1.getY()
              << ")\n";
    std::cout << "Particle 2 Position: (" << p2.getX() << ", " << p2.getY()
              << ")\n";

    window.clear();
    window.draw(shape1);
    window.draw(shape2);
    window.display();
  }
  return 0;
}