#pragma once
#include "Path.h"


enum SplineObjectType {
    Joint = 0,
    Joint_Ctrl,
    None,
};

struct PathEditor: public sf::Drawable {
    PathEditor(Path* p): current_editing_path{ p } {}
    void handle_event(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
    void update_joint(Vec2f new_position);
    void update_ctrl_joint(Vec2f new_position);

    bool on_editting = true;
    size_t on_moving_index          = 0;
    SplineObjectType on_moving_type = None;
    Path* current_editing_path;
};
