#include <iostream>
#include "PathEditor.h"
#define SCROLL_SENSITIVITY 20
#define FPS 60
#define DELTA_TIME  1.0f/FPS
#define ZOOM_FACTOR 0.1f
#define FONT_PATH "./VictorMono.ttf"

inline void draw_help(sf::RenderTarget& target, const sf::Font& font, Vec2f position);

Config config;
inline bool on_control = false;
inline bool show_help = true;
int main() {
    config.read("config");
    config.print();

    srand(time(0));
    Path path;
    PathEditor path_editor(&path);
    sf::RenderWindow window(sf::VideoMode(config.screen_w, config.screen_h), "PathEditor");
    sf::Event event;
    sf::Font font;
    font.loadFromFile(FONT_PATH);

    sf::Clock clock;
    while (window.isOpen()) {
        clock.restart();
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Resized: {
                Vec2f new_size(event.size.width, event.size.height);
                sf::View view(new_size/2.0f, new_size);
                window.setView(view);
            } break;
            case sf::Event::MouseWheelScrolled: {
                auto view = window.getView();
                auto center = view.getCenter();
                auto& wheel_event = event.mouseWheelScroll;
                if (!on_control) {
                    if (wheel_event.wheel == sf::Mouse::VerticalWheel)
                        center.y -= wheel_event.delta * SCROLL_SENSITIVITY;
                    else if (wheel_event.wheel == sf::Mouse::HorizontalWheel)
                        center.x -= wheel_event.delta * SCROLL_SENSITIVITY;
                    else
                        assert(false);
                    view.setCenter(center);
                    window.setView(view);
                }
                else if (wheel_event.wheel == sf::Mouse::VerticalWheel) {
                    Vec2f mouse_pos = (Vec2f)sf::Mouse::getPosition(window);
                    path.zoom(Helper::to_global_position(window, mouse_pos), 1.0f + wheel_event.delta * ZOOM_FACTOR); // delta: 1 or -1
                }
            } break;
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::LControl:
                    on_control = true;
                    break;
                case sf::Keyboard::S:
                    if (on_control) path.save("Testingpath");
                    break;
                case sf::Keyboard::L:
                    if (on_control) path.load("Testingpath");
                    break;
                case sf::Keyboard::T:
                    show_help = !show_help;
                    break;
                default: break;
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
        if (show_help) draw_help(window, font, Helper::to_global_position(window, Vec2f(0, 0)));
        window.display();
        auto elapsed_time = clock.getElapsedTime();
        if (elapsed_time.asSeconds() < DELTA_TIME) {
            sf::sleep(sf::seconds(DELTA_TIME) - clock.getElapsedTime());
        }
    }
}

inline void draw_help(sf::RenderTarget& target, const sf::Font& font, Vec2f position) {
    sf::Text text("\
t: toogle help\n\
a: switch to adding mode\n\
s: switch to editing mode\n\
scroll horizontal/vertical to move view\n\
hold CTRL and scroll vertical to zoom", font);
    text.setPosition(position);
    target.draw(text);
}
