#include <iostream>
#include <cstring>
#include "PathEditor.h"
#include "Config.h"
#define SCROLL_SENSITIVITY 20
#define DEFAULT_PATH_COLOR 0x48596566
#define FPS 60
#define ZOOM_FACTOR 0.1f
#define FONT_PATH "./VictorMono.ttf"

inline char* shift_args(int *argc, char ***argv) {
    if (*argc > 0) {
        (*argc)--;
        char *result = **argv;
        (*argv)++;
        return result;
    }
    return nullptr;
}

void usage(const char* prog) {
    printf("Usage: %s [OPTIONS]\n", prog);
    printf("OPTIONS:\n");
    printf("    -o {output_path}   [default to 'output.path']\n");
    printf("    -i {file to edit}                            \n");
    printf("    -c {config_file}                             \n");
}

inline void draw_help(sf::RenderTarget& target, const sf::Font& font, sf::Vector2i position);
Config config;
inline bool on_control = false;
inline bool show_help = true;
inline Vec2f start_drag {-1, -1};
int main(int argc, char **argv) {
    srand(time(0));

    const char *output_file = "output.path";
    const char *input_file = nullptr;
    const char *config_file = nullptr;

    const char *prog = shift_args(&argc, &argv);
    while (argc > 0) {
        const char *arg = shift_args(&argc, &argv);
        if (strcmp(arg, "--help") == 0) {
            usage(prog);
            return 0;
        } else if (argc == 0) {
            usage(prog);
            return 1;
        } else if (strcmp(arg, "-i") == 0) {
            input_file = shift_args(&argc, &argv);
        } else if (strcmp(arg, "-o") == 0) {
            output_file = shift_args(&argc, &argv);
        } else if (strcmp(arg, "-c") == 0) {
            config_file = shift_args(&argc, &argv);
        } else {
            fprintf(stderr, "Error: Unknown option %s\n", arg);
            usage(prog);
            return 1;
        }
    }

    if (config_file) config.read(config_file);
    config.print();

    Path path(config.path_width, config_file ? config.path_color.toInteger() : DEFAULT_PATH_COLOR);
    PathEditor path_editor(&path);

    if (input_file) path.load(input_file);

    sf::RenderWindow window(sf::VideoMode(config.screen_w, config.screen_h), "PathEditor");
    window.setFramerateLimit(FPS);
    sf::Event event;
    sf::Font font;
    font.loadFromFile(FONT_PATH);

    sf::Clock clock;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized: {
                Vec2f new_size(event.size.width, event.size.height);
                sf::View view(new_size/2.0f, new_size);
                window.setView(view);
            } break;
            case sf::Event::MouseWheelScrolled: {
                auto& wheel_event = event.mouseWheelScroll;
                if (wheel_event.wheel == sf::Mouse::VerticalWheel) {
                    auto mouse_pos = sf::Mouse::getPosition(window);
                    Helper::zoom(window, mouse_pos, 1.0f + wheel_event.delta * ZOOM_FACTOR); // delta: 1 or -1
                }
            } break;
            case sf::Event::MouseButtonPressed: {
                Vec2f mouse_pos = (Vec2f)sf::Mouse::getPosition(window);
                if (event.mouseButton.button == sf::Mouse::Right)
                    start_drag = mouse_pos;
            } break;
            case sf::Event::MouseButtonReleased: {
                if (event.mouseButton.button == sf::Mouse::Right)
                    start_drag = {-1, -1};
            } break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::LControl:
                    on_control = true;
                    break;
                case sf::Keyboard::S:
                    if (on_control) {
                        if (path.save(output_file)) {
                            printf("Saved to file '%s'\n", output_file);
                        }
                    }
                    break;
                case sf::Keyboard::R:
                    if (on_control) {
                        if (path.load(output_file)) {
                            printf("Loaded from file '%s'\n", output_file);
                        }
                    }
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
        if (start_drag.x != -1) {
            Vec2f mouse_pos = (Vec2f)sf::Mouse::getPosition(window);
            Vec2f vel = mouse_pos - start_drag;
            auto view = window.getView();
            view.setCenter(view.getCenter() - vel);
            window.setView(view);
            start_drag = mouse_pos;
        }
        window.clear();
        window.draw(path);
        window.draw(path_editor);
        if (show_help) draw_help(window, font, sf::Vector2i(0, 0));
        window.display();
    }
}

inline void draw_help(sf::RenderTarget& target, const sf::Font& font, sf::Vector2i position) {
    static sf::Text text("\
  t: toogle help\n\
  a: switch to adding mode\n\
  s: switch to editing mode\n\
C-s: save to file\n\
C-r: load back from previous saved file\n\
Hold mouse right button and drag to move screen\n\
Scroll up/down to zoom", font);
    auto tmp_view = target.getView();
    target.setView(target.getDefaultView());
    text.setPosition(target.mapPixelToCoords(position));
    target.draw(text);
    target.setView(tmp_view);
}
