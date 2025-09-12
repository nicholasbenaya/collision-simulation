#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "Collision Sim");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.display();
    }
    return 0;
}