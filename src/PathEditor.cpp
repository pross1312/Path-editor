#include "PathEditor.h"
#include "Config.h"

extern Config config;

void PathEditor::handle_moving_event(sf::Event& event, sf::RenderWindow& window) {
    static bool on_clicked = false;
    auto& joints      = current_editing_path->spline.joints;
    auto& joint_ctrls = current_editing_path->spline.joint_ctrls;
    Vec2f mouse_pos   = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (!on_clicked) object.type = None;
    for (size_t i = 0; i < joints.size(); i++) {
        if (Helper::distance(joints[i], mouse_pos) <= config.joint_radius) {
            object.index = i;
            object.type = Joint;
        }
    }
    if (object.type == None) {
        for (size_t i = 0; i < joint_ctrls.size(); i++) {
            Vec2f size(config.ctrl_point_size, config.ctrl_point_size);
            sf::Rect<float> rect(joint_ctrls[i] - size/2.0f, size);
            if (rect.contains(mouse_pos)) {
                object.index = i;
                object.type = Joint_Ctrl;
            }
        }
    }
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left) on_clicked = true;
        break;
    case sf::Event::MouseButtonReleased:
        on_clicked = false;
        break;
    default: break;
    }
    if (on_clicked && object.type == Joint) update_joint(mouse_pos);
    else if (on_clicked && object.type == Joint_Ctrl) update_ctrl_joint(mouse_pos);
}

void PathEditor::handle_adding_event(sf::Event& event, sf::RenderWindow& window) {
    const auto& vArray = current_editing_path->spline.vArray;
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        if (object.type == Vertex && event.mouseButton.button == sf::Mouse::Left) {
            auto& joints              = current_editing_path->spline.joints;
            auto& joint_ctrls         = current_editing_path->spline.joint_ctrls;
            size_t vertices_per_curve = current_editing_path->spline.vertices_per_curve;
            size_t joint_index        = object.index/vertices_per_curve + 1;
            float len                 = 100.0f;
            auto b                    = vArray[(object.index+1)%vArray.getVertexCount()].position;
            auto a                    = vArray[object.index].position;
            Vec2f tangent_vec         = len*Helper::normalized(b - a);
            Vec2f start_ctrl          = a - tangent_vec;
            Vec2f end_ctrl            = a + tangent_vec;
            joints.insert(joints.begin() + joint_index, a);
            joint_ctrls.insert(joint_ctrls.begin() + joint_index*2, start_ctrl);
            joint_ctrls.insert(joint_ctrls.begin() + joint_index*2+1, end_ctrl);
            current_editing_path->update();
        }
        break;
    default: break;
    }
    Vec2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (current_editing_path->contains(mouse_pos)) {
        float min_dis = 1e10;
        for (size_t i = 0; i < vArray.getVertexCount(); i++) {
            float temp_dis = Helper::distance(mouse_pos, vArray[i].position);
            if (temp_dis <= min_dis) {
                object.index = i;
                object.type = Vertex;
                min_dis = temp_dis;
            }
        }
    } else object.type = None;
}

void PathEditor::handle_event(sf::Event& event, sf::RenderWindow& window) {
    switch (event.type) {
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::S) mode = Moving;
            else if (event.key.code == sf::Keyboard::A) mode = Adding;
            break;
        default: break;
    }
    if (mode == Moving) handle_moving_event(event, window);
    else if (mode == Adding) handle_adding_event(event, window);
}

void PathEditor::update_ctrl_joint(Vec2f new_position) {
    auto& joint_ctrls             = current_editing_path->spline.joint_ctrls;
    auto& joint                   = current_editing_path->spline.joints[object.index/2];
    size_t opposite_index         = object.index + (object.index%2 == 0 ? 1 : -1);
    float opposite_joint_ctrl_len = Helper::distance(joint, joint_ctrls[opposite_index]);
    joint_ctrls[object.index]     = new_position;
    joint_ctrls[opposite_index]   = joint + Helper::normalized(joint - new_position) * opposite_joint_ctrl_len;
    current_editing_path->update();
}

void PathEditor::update_joint(Vec2f new_position) {
    auto& joints                   = current_editing_path->spline.joints;
    auto& joint_ctrls              = current_editing_path->spline.joint_ctrls;
    Vec2f vel                      = new_position - joints[object.index];
    joints[object.index]           = new_position;
    joint_ctrls[2*object.index]   += vel;
    joint_ctrls[2*object.index+1] += vel;
    current_editing_path->update();
}

void PathEditor::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    switch (mode) {
    case Adding: {
        draw_all_object(target, state);
        if (object.type == Vertex) {
            const auto& vArray = current_editing_path->spline.vArray;
            sf::CircleShape c(config.joint_radius);
            c.setOrigin(config.joint_radius, config.joint_radius);
            c.setPosition(vArray[object.index].position);
            c.setFillColor(sf::Color(ON_EDITING_OBJECT_COLOR));
            auto b            = vArray[(object.index+1)%vArray.getVertexCount()].position;
            auto a            = vArray[object.index].position;
            float len         = 100.0f;
            Vec2f tangent_vec = len*Helper::normalized(b - a);
            Vec2f start_ctrl  = a - tangent_vec;
            Vec2f end_ctrl    = a + tangent_vec;
            target.draw(c, state);
            Helper::draw_line(target, a, start_ctrl, sf::Color(ON_EDITING_OBJECT_COLOR));
            Helper::draw_line(target, a, end_ctrl, sf::Color(ON_EDITING_OBJECT_COLOR));
        }
    } break;
    case Moving: {
        if (object.type == None) draw_all_object(target, state);
        else if (object.type == Joint) draw_all_object(target, state, object.index);
        else if (object.type == Joint_Ctrl) draw_all_object(target, state, object.index/2);
    } break;
    default: break;
    }
}

void PathEditor::draw_all_object(sf::RenderTarget& target, sf::RenderStates state, std::optional<size_t> on_editting_joint) const {
    auto& joints = current_editing_path->spline.joints;
    auto& joint_ctrls = current_editing_path->spline.joint_ctrls;
    for (size_t i = 0; i < joint_ctrls.size(); i++) {
        Vec2f size{ config.ctrl_point_size, config.ctrl_point_size };
        sf::RectangleShape ctrl(size);
        ctrl.setOrigin(size / 2.0f);
        ctrl.setPosition(joint_ctrls[i]);
        if (on_editting_joint && on_editting_joint.value() == i/2) ctrl.setFillColor(sf::Color(ON_EDITING_OBJECT_COLOR));
        else ctrl.setFillColor(config.ctrl_point_color);
        target.draw(ctrl, state);
    }
    for (size_t i = 0; i < joints.size(); i++) {
        sf::CircleShape joint(config.joint_radius);
        joint.setOrigin(config.joint_radius, config.joint_radius);
        joint.setPosition(joints[i]);
        if (on_editting_joint && on_editting_joint.value() == i) joint.setFillColor(sf::Color(ON_EDITING_OBJECT_COLOR));
        else joint.setFillColor(config.joint_color);
        Helper::draw_line(target,
                joints[i],
                joint_ctrls[2*i],
                on_editting_joint && on_editting_joint.value() == i ? sf::Color(ON_EDITING_OBJECT_COLOR) : config.line_color);
        Helper::draw_line(target,
                joints[i],
                joint_ctrls[2*i+1],
                on_editting_joint && on_editting_joint.value() == i ? sf::Color(ON_EDITING_OBJECT_COLOR) : config.line_color);
        target.draw(joint, state);
    }
}
