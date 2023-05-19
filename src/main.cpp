#include <iostream>
#include "Path.h"
inline const unsigned WIDTH = 1920;
inline const unsigned HEIGHT = 1080;

int main(int argc, char** argv) {
    Config::read_config("config");
    std::cout << "************************************************" << std::endl;
    std::cout << "Program will read configuration in config file." << std::endl;
    std::cout << "To change some properties, modify it in config file." << std::endl;
    std::cout << "Press 'r' to read a path from file." << std::endl;
    std::cout << "Press 'w' to write current editing path to file." << std::endl;
    std::cout << "************************************************" << std::endl;

    Config::print_config();
    srand(time(0));
    sf::RenderWindow _window{ sf::VideoMode(WIDTH, HEIGHT), "PathEditor" };
    sf::Event _event{};
    // Spline spline{initNCurves};
    Path path{};
    while (_window.isOpen()) {
        while (_window.pollEvent(_event)) {
            if (_event.type == sf::Event::Closed)
                _window.close();
            path.onUserEditing(_event, _window);
        }
        

        _window.clear(Config::back_ground);

        _window.draw(path);


        _window.display();
    }
}
