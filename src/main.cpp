#include <iostream>
#include "PathEditor.h"
Config config;
bool on_control = false;
int main() {
    config.read("config");
    config.print();
    std::cout << "************************************************" << std::endl;
    std::cout << "Program will read configuration in config file." << std::endl;
    std::cout << "To change some properties, modify it in config file." << std::endl;
    std::cout << "Press 'r' to read a path from file." << std::endl;
    std::cout << "Press 'w' to write current editing path to file." << std::endl;
    std::cout << "************************************************" << std::endl;

    srand(time(0));
    sf::RenderWindow window{ sf::VideoMode(config.screen_w, config.screen_h), "PathEditor" };
    sf::Event event{};
    Path path;
    PathEditor path_editor(&path);
    // std::fstream fout("Checking");
    // fout << path.spline;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::LControl) {
                    on_control = true;
                }
                else if (event.key.code == sf::Keyboard::S && on_control) {
                    path.save("Testingpath");
                }
                else if (event.key.code == sf::Keyboard::L && on_control) {
                    path.load("Testingpath");
                }
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::LControl) {
                    on_control = false;
                }
                break;
            default: break;
            }
            path_editor.handle_event(event, window);
        }
        window.clear();
        window.draw(path);
        window.draw(path_editor);
        window.display();
    }
    // fout.close();
}
