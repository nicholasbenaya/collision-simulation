#include "Particle.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

// Function to generate particles randomly
vector<Particle> generateParticles(int count, double windowWidth, double windowHeight) {
    vector<Particle> particles;
    
    // Random number generators
    random_device rd;
    mt19937 gen(rd());
    
    // Calculate appropriate mass range based on particle count and window size
    double windowArea = windowWidth * windowHeight;
    double maxTotalParticleArea = windowArea * 0.3; // Use only 30% of screen area
    double averageParticleArea = maxTotalParticleArea / count;
    double averageRadius = sqrt(averageParticleArea / M_PI);
    
    // Mass calculation: radius = sqrt(mass) * 20, so mass = (radius/20)^2
    double maxMass = pow(averageRadius / 20.0, 2);
    double minMass = maxMass * 0.1; // Minimum mass is 10% of max
    
    // Ensure reasonable bounds
    if (maxMass < 0.1) maxMass = 0.1;
    if (minMass < 0.01) minMass = 0.01;
    if (maxMass > 10.0) maxMass = 10.0; // Cap maximum mass
    
    uniform_real_distribution<double> massDist(minMass, maxMass);
    uniform_real_distribution<double> velocityDist(-100.0, 100.0);
    
    for (int i = 0; i < count; ++i) {
        double mass = massDist(gen);
        double radius = sqrt(mass) * 20;
        
        // Ensure position bounds account for particle radius
        double minX = radius;
        double maxX = windowWidth - radius;
        double minY = radius;
        double maxY = windowHeight - radius;
        
        // Safety check - if window is too small, use smaller particles
        if (minX >= maxX || minY >= maxY) {
            mass = min(mass, 0.01); // Force smaller mass
            radius = sqrt(mass) * 20;
            minX = radius;
            maxX = windowWidth - radius;
            minY = radius;
            maxY = windowHeight - radius;
        }
        
        uniform_real_distribution<double> xDist(minX, max(minX + 1, maxX));
        uniform_real_distribution<double> yDist(minY, max(minY + 1, maxY));
        
        double x = xDist(gen);
        double y = yDist(gen);
        
        // Create particle
        Particle particle(x, y, mass);
        
        // Set random velocity
        double vx = velocityDist(gen);
        double vy = velocityDist(gen);
        particle.setVelocity(vx, vy);
        
        particles.push_back(particle);
    }
    
    return particles;
}

// Function to create shapes for particles
vector<sf::CircleShape> createParticleShapes(const vector<Particle>& particles) {
    vector<sf::CircleShape> shapes;
    
    for (size_t i = 0; i < particles.size(); ++i) {
        const Particle& p = particles[i];
        sf::CircleShape shape(p.getRadius());
        
        // Set different colors based on mass or index
        sf::Uint8 red = static_cast<sf::Uint8>((i * 50) % 255);
        sf::Uint8 green = static_cast<sf::Uint8>((i * 100) % 255);
        sf::Uint8 blue = static_cast<sf::Uint8>((i * 150) % 255);
        shape.setFillColor(sf::Color(red, green, blue));
        
        shape.setOrigin(shape.getRadius(), shape.getRadius());
        shape.setPosition(p.getX(), p.getY());
        
        shapes.push_back(shape);
    }
    
    return shapes;
}

void clampParticleToWindow(Particle &particle, double windowWidth, double windowHeight) {
    double radius = particle.getRadius();

    // Get current position
    double x = particle.getX();
    double y = particle.getY();

    // Clamp position to window bounds
    if (x < radius) {
        particle.setPosition(radius, y);
    } else if (x > windowWidth - radius) {
        particle.setPosition(windowWidth - radius, y);
    }

    if (y < radius) {
        particle.setPosition(particle.getX(), radius);
    } else if (y > windowHeight - radius) {
        particle.setPosition(particle.getX(), windowHeight - radius);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Collision Sim",
                            sf::Style::Resize | sf::Style::Close);

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

    // Generate particles - adjust count as needed
    int particleCount = 300; // Start with 50, you can increase up to 100k
    vector<Particle> particles = generateParticles(particleCount, window.getSize().x, window.getSize().y);
    vector<sf::CircleShape> shapes = createParticleShapes(particles);

    particleCountText.setString("Particles: " + to_string(particleCount));

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        window.setVerticalSyncEnabled(false);
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle window resize
            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

                fpsText.setPosition(event.size.width - 100, 10);

                // Clamp all particles to new window bounds
                for (auto& particle : particles) {
                    clampParticleToWindow(particle, event.size.width, event.size.height);
                }
            }
            
            // Add more particles with spacebar
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                vector<Particle> newParticles = generateParticles(50, window.getSize().x, window.getSize().y);
                vector<sf::CircleShape> newShapes = createParticleShapes(newParticles);
                
                particles.insert(particles.end(), newParticles.begin(), newParticles.end());
                shapes.insert(shapes.end(), newShapes.begin(), newShapes.end());
                
                particleCount += 50;
                particleCountText.setString("Particles: " + to_string(particleCount));
            }
        }

        double dt = clock.restart().asSeconds();
        if (dt > 0.5)
            dt = 0.5;

        double fps = 1.0 / dt;
        fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

        // Update all particles
        sf::Vector2u windowSize = window.getSize();
        for (auto& particle : particles) {
            particle.update(dt, windowSize.x, windowSize.y);
        }

        // Check collisions between all particles (simple O(n²) approach)
        // For 100k particles, you'd want spatial partitioning for performance
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                if (particles[i].isColliding(particles[j])) {
                    particles[i].resolveCollision(particles[j]);
                }
            }
        }

        // Update shapes' positions
        for (size_t i = 0; i < particles.size(); ++i) {
            shapes[i].setPosition(particles[i].getX(), particles[i].getY());
        }

        // Render
        window.clear();
        for (const auto& shape : shapes) {
            window.draw(shape);
        }
        window.draw(fpsText);
        window.draw(particleCountText);
        window.display();
    }

    return 0;
}