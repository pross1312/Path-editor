#include <iostream>
#include "Path.h"

int main() {
    Config::read_config("config");
    std::cout << "************************************************" << std::endl;
    std::cout << "Program will read configuration in config file." << std::endl;
    std::cout << "To change some properties, modify it in config file." << std::endl;
    std::cout << "Press 'r' to read a path from file." << std::endl;
    std::cout << "Press 'w' to write current editing path to file." << std::endl;
    std::cout << "************************************************" << std::endl;

    Config::print_config();
    srand(time(0));
    sf::RenderWindow window{ sf::VideoMode(Config::screen_w, Config::screen_h), "PathEditor" };
    sf::Event event{};
    Path path;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            path.on_user_editting(event, window);
        }
        window.clear();
        window.draw(path);
        window.display();
    }
}
