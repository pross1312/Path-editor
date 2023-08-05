#include "PathEditor.h"

extern Config config;

void PathEditor::handle_event(sf::Event& event, sf::RenderWindow& window) {
    Vec2f mouse_pos                        = (Vec2f)sf::Mouse::getPosition(window);
    auto& joints                           = current_editing_path->spline.joints;
    auto& joint_ctrls                      = current_editing_path->spline.joint_ctrls;
    switch (event.type) {
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::T) on_editting = !on_editting;
            if (event.key.code == sf::Keyboard::Delete) on_editting = !on_editting;
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left && on_editting) {
                for (size_t i = 0; i < joints.size(); i++) {
                    if (Helper::distance(joints[i], mouse_pos) <= config.joint_radius) {
                        on_moving_index = i;
                        on_moving_type = Joint;
                    }
                }
                if (on_moving_type == None) {
                    for (size_t i = 0; i < joint_ctrls.size(); i++) {
                        Vec2f size(config.ctrl_point_size, config.ctrl_point_size);
                        sf::Rect<float> rect(joint_ctrls[i] - size/2.0f, size);
                        if (rect.contains(mouse_pos)) {
                            on_moving_index = i;
                            on_moving_type = Joint_Ctrl;
                        }
                    }
                    printf("%zu\n", on_moving_index);
                }
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (on_editting && on_moving_type != None && event.mouseButton.button == sf::Mouse::Left) on_moving_type = None;
            break;
        default:
            break;
    }
    if (on_moving_type == Joint) update_joint(mouse_pos);
    else if (on_moving_type == Joint_Ctrl) update_ctrl_joint(mouse_pos);
}

void PathEditor::update_ctrl_joint(Vec2f new_position) {
    auto& joint_ctrls             = current_editing_path->spline.joint_ctrls;
    auto& joint                   = current_editing_path->spline.joints[on_moving_index/2];
    size_t opposite_index         = on_moving_index + (on_moving_index%2 == 0 ? 1 : -1);
    float opposite_joint_ctrl_len = Helper::distance(joint, joint_ctrls[opposite_index]);
    joint_ctrls[on_moving_index]  = new_position;
    joint_ctrls[opposite_index]   = joint + Helper::normalized(joint - new_position) * opposite_joint_ctrl_len;
    current_editing_path->update();
    current_editing_path->spline.update();
}

void PathEditor::update_joint(Vec2f new_position) {
    auto& joints                           = current_editing_path->spline.joints;
    auto& joint_ctrls                      = current_editing_path->spline.joint_ctrls;
    Vec2f vel                         = new_position - joints[on_moving_index];
    joints[on_moving_index]           = new_position;
    joint_ctrls[2*on_moving_index]   += vel;
    joint_ctrls[2*on_moving_index+1] += vel;
    current_editing_path->spline.update();
    current_editing_path->update();
}

void PathEditor::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    if (!on_editting) return;
    auto& joints = current_editing_path->spline.joints;
    auto& joint_ctrls = current_editing_path->spline.joint_ctrls;
    for (size_t i = 0; i < joint_ctrls.size(); i++) {
        Vec2f size{ config.ctrl_point_size, config.ctrl_point_size };
        sf::RectangleShape r(size);
        if ((on_moving_type == Joint      && i/2 == on_moving_index  ) ||
                (on_moving_type == Joint_Ctrl && i/2 == on_moving_index/2))
            r.setFillColor(sf::Color(0xff0000ff));
        else
            r.setFillColor(config.ctrl_point_color);
        r.setOrigin(size / 2.0f);
        r.setPosition(joint_ctrls[i]);
        target.draw(r);
    }
    for (size_t i = 0; i < joints.size(); i++) {
        sf::CircleShape c(config.joint_radius);
        c.setOrigin(config.joint_radius, config.joint_radius);
        c.setPosition(joints[i]);
        if ((on_moving_type == Joint      && i == on_moving_index  ) ||
                (on_moving_type == Joint_Ctrl && i == on_moving_index/2))
            c.setFillColor(sf::Color(0xff0000ff));
        else
            c.setFillColor(config.joint_color);
        target.draw(c, state);
        Helper::draw_line(target, joints[i], joint_ctrls[2*i]  , config.line_color);
        Helper::draw_line(target, joints[i], joint_ctrls[2*i+1], config.line_color);
    }
}
