#pragma once
#include "Path.h"
#include <optional>

#define ON_EDITING_OBJECT_COLOR 0xff7279ff

enum SplineObjectType {
    Joint = 0,
    Joint_Ctrl,
    Vertex,
    None,
};
enum EdittingMode {
    Moving,
    Adding,
    Viewing,
};
struct EdittingObject {
    size_t index = 0;
    SplineObjectType type = None;
};

struct PathEditor: public sf::Drawable {
    PathEditor(Path* p): current_editing_path{ p } {}
    void handle_event(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
    void update_joint(Vec2f new_position);
    void update_ctrl_joint(Vec2f new_position);
    void handle_moving_event(sf::Event& event, sf::RenderWindow& window);
    void handle_adding_event(sf::Event& event, sf::RenderWindow& window);
    void draw_all_object(sf::RenderTarget& target, sf::RenderStates state, std::optional<size_t> on_editting_joint = std::nullopt) const;


    EdittingMode mode = Moving;
    EdittingObject object;
    Path* current_editing_path;
};
