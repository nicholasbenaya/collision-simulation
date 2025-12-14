#include "BruteForceDetector.hpp"
#include "Particle.hpp"
#include "QuadTreeDetector.hpp"
#include "Simulation.hpp"
#include "SpatialHashDetector.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// Refactored: moved particle generation, shapes, clamping, and collision
// handling into `Simulation` and detectors modules.

int main() {
  // Start windowed to avoid driver stalls on some setups
  sf::RenderWindow window(sf::VideoMode(1280, 720), "Collision Sim",
                          sf::Style::Default);
  // Set vsync once, not per frame
  window.setVerticalSyncEnabled(false);

  sf::Font font;
  if (!font.loadFromFile("assets/font/MontserratBlack-3zOvZ.ttf")) {
    std::cerr << "Failed to load font\n";
    return -1;
  }

  sf::Text fpsText;
  fpsText.setFont(font);
  fpsText.setCharacterSize(20);
  fpsText.setFillColor(sf::Color::Green);

  sf::Text particleCountText;
  particleCountText.setFont(font);
  particleCountText.setCharacterSize(16);
  particleCountText.setFillColor(sf::Color::Yellow);
  particleCountText.setPosition(10, 10);

  sf::Text algoText;
  algoText.setFont(font);
  algoText.setCharacterSize(16);
  algoText.setFillColor(sf::Color::Cyan);
  algoText.setPosition(10, 55);

  sf::Text controlsText;
  controlsText.setFont(font);
  controlsText.setCharacterSize(14);
  controlsText.setFillColor(sf::Color::White);
  controlsText.setPosition(10, 35);
  controlsText.setString("Controls: + (add 500) | - (remove 500) | B (Brute) | "
                         "Q (QuadTree) | H (SpatialHash)");

  // Simulation setup
  Simulation sim(window);
  sim.setDetector(std::unique_ptr<ICollisionDetector>(new BruteForceDetector()),
                  "Brute Force");
  int initialCount = 300;
  sim.generateParticles(initialCount);
  particleCountText.setString("Particles: " + to_string(initialCount));
  algoText.setString(std::string("Algorithm: ") + sim.currentAlgorithmName());

  sf::Clock clock;

  bool isFullscreen = false;
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      // Handle window resize
      if (event.type == sf::Event::Resized) {
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));

        fpsText.setPosition(event.size.width - 100, 10);
      }

      // Handle keyboard input for adding/removing particles
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
          window.close();
        } else if (event.key.code == sf::Keyboard::F) {
          // Toggle fullscreen/windowed
          isFullscreen = !isFullscreen;
          sf::Uint32 style =
              isFullscreen ? sf::Style::Fullscreen : sf::Style::Default;
          // Recreate window with new style
          window.create(isFullscreen ? sf::VideoMode::getDesktopMode()
                                     : sf::VideoMode(1280, 720),
                        "Collision Sim", style);
          window.setVerticalSyncEnabled(false);
          // Reset view to new size
          sf::FloatRect visibleArea(0, 0, window.getSize().x,
                                    window.getSize().y);
          window.setView(sf::View(visibleArea));
          fpsText.setPosition(window.getSize().x - 100, 10);
        } else if (event.key.code == sf::Keyboard::Equal ||
                   event.key.code == sf::Keyboard::Add) {
          sim.addParticles(500);
          particleCountText.setString("Particles: " +
                                      to_string(sim.particleCount()));
        } else if (event.key.code == sf::Keyboard::Hyphen ||
                   event.key.code == sf::Keyboard::Subtract) {
          sim.removeOldest(500);
          particleCountText.setString("Particles: " +
                                      to_string(sim.particleCount()));
        } else if (event.key.code == sf::Keyboard::B) {
          sim.setDetector(
              std::unique_ptr<ICollisionDetector>(new BruteForceDetector()),
              "Brute Force");
          algoText.setString(std::string("Algorithm: ") +
                             sim.currentAlgorithmName());
        } else if (event.key.code == sf::Keyboard::Q) {
          sim.setDetector(
              std::unique_ptr<ICollisionDetector>(new QuadTreeDetector()),
              "QuadTree");
          algoText.setString(std::string("Algorithm: ") +
                             sim.currentAlgorithmName());
        } else if (event.key.code == sf::Keyboard::H) {
          sim.setDetector(std::unique_ptr<ICollisionDetector>(
                              new SpatialHashDetector(80.0)),
                          "SpatialHash");
          algoText.setString(std::string("Algorithm: ") +
                             sim.currentAlgorithmName());
        }
      }
    }

    double dt = clock.restart().asSeconds();
    if (dt > 0.5)
      dt = 0.5;

    double fps = 1.0 / dt;
    fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

    // Update and collision via Simulation
    sim.update(dt);

    // Render
    window.clear();
    sim.render();
    window.draw(fpsText);
    window.draw(particleCountText);
    window.draw(algoText);
    window.draw(controlsText);
    window.display();
  }

  return 0;
}