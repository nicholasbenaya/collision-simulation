#include "Particle.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

#include <cmath>

using namespace std;
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Collision Sim");
//   sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Collision Sim",
//                           sf::Style::Fullscreen);
  sf::Font font;
  if (!font.loadFromFile("/mnt/c/Users/mahdi/Desktop/Dasprog/Collision "
                         "SImulation/assets/font/MontserratBlack-3zOvZ.ttf")) {
    std::cerr << "Failed to load font\n";
    return -1;
  }
  sf::Text fpsText;
  fpsText.setFont(font);
  fpsText.setCharacterSize(20);
  fpsText.setFillColor(sf::Color::Green);
  fpsText.setPosition(window.getSize().x - 100, 10); // Top-right corner

  // Particle object creation
  Particle p1(0, 0, 5.0);
  Particle p2(200, 200, 10.0);

  sf::CircleShape shape1(sqrt(p1.getMass()) *
                         20); // Radius proportional to mass
  shape1.setFillColor(sf::Color::Red);
  shape1.setOrigin(shape1.getRadius(),
                   shape1.getRadius()); // Set origin to center
  shape1.setPosition(p1.getX(), p1.getY());

  sf::CircleShape shape2(sqrt(p2.getMass()) *
                         20); // Radius proportional to mass
  shape2.setFillColor(sf::Color::Blue);
  shape2.setOrigin(shape2.getRadius(),
                   shape2.getRadius()); // Set origin to center
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
    double fps = 1.0 / dt;
    fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

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
    window.draw(fpsText);
    window.display();
  }
  return 0;
}