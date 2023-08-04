#include "PathEditor.h"

enum SplineObjectType {
    Joint = 0,
    Joint_Ctrl,
    None,
};

void PathEditor::handle_event(sf::Event& event, sf::RenderWindow& window) {
    static size_t on_moving_index          = 0;
    static SplineObjectType on_moving_type = None;
    Vec2f mouse_pos                        = (Vec2f)sf::Mouse::getPosition(window);
    auto& joints                           = current_editing_path->spline.joints;
    auto& joint_ctrls                      = current_editing_path->spline.joint_ctrls;
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        for (size_t i = 0; i < joints.size(); i++) {
            if (Helper::distance(joints[i], mouse_pos) <= Config::joint_radius) {
                on_moving_index = i;
                on_moving_type = Joint;
            }
        }
        if (on_moving_type == None) {
            for (size_t i = 0; i < joint_ctrls.size(); i++) {
                Vec2f size(Config::ctrl_point_size, Config::ctrl_point_size);
                sf::Rect<float> rect(joint_ctrls[i] - size/2.0f, size);
                if (rect.contains(mouse_pos)) {
                    on_moving_index = i;
                    on_moving_type = Joint_Ctrl;
                }
            }
        }
    }
    else if (on_moving_type != None && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        on_moving_type = None;
    }
    if (on_moving_type == Joint) {
        Vec2f vel                         = mouse_pos - joints[on_moving_index];
        joints[on_moving_index]           = mouse_pos;
        joint_ctrls[2*on_moving_index]   += vel;
        joint_ctrls[2*on_moving_index+1] += vel;
        current_editing_path->spline.update();
        current_editing_path->update();
    }
    else if (on_moving_type == Joint_Ctrl) {
        auto joint                    = joints[on_moving_index/2];
        size_t opposite_index         = on_moving_index + (on_moving_index%2 == 0 ? 1 : -1);
        float opposite_joint_ctrl_len = Helper::distance(joint, joint_ctrls[opposite_index]);
        joint_ctrls[on_moving_index]  = mouse_pos;
        joint_ctrls[opposite_index]   = joint + Helper::normalized(joint - mouse_pos) * opposite_joint_ctrl_len;
        current_editing_path->spline.update();
        current_editing_path->update();
    }
}
