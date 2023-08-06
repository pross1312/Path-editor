#include "Spline.h"
#include "Helper.h"
#include <exception>
#include <iostream>
#define PTR std::make_shared
using Vec2f = sf::Vector2f;
extern Config config;

Spline::Spline(size_t nJoints, size_t vertices_per_curve):
    vertices_per_curve { vertices_per_curve },
    joints             { nJoints   }, // start - end - start - end - ...
    joint_ctrls        { nJoints*2 },
    vArray             { sf::LineStrip, nJoints * vertices_per_curve } {
    size_t w = config.screen_w;
    size_t h = config.screen_h;
    float angle = M_PI;
    float angle_step = 2.0*M_PI / nJoints;
    float radius = 400;
    float dis = 80;
    for (size_t i = 0; i < nJoints; i++) {
        float x = std::cos(angle)*radius + w/2.0f;
        float y = std::sin(angle)*radius + h/2.0f;
        joints[i] = Vec2f{ x, y };
        angle += angle_step;

        Vec2f rp = joints[i] - Vec2f{ w/2.0f, h/2.0f };
        rp = Helper::normalized(rp);
        rp = Helper::rotated(rp, 90.0f);
        joint_ctrls[2*i]     = joints[i] + rp*dis;
        joint_ctrls[2*i + 1] = joints[i] - rp*dis;
    }
    update();
}

void Spline::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    target.draw(vArray, state);
}

void Spline::update() {
    if (vArray.getVertexCount() != joints.size()*vertices_per_curve) {
        vArray.resize(joints.size() * vertices_per_curve);
    }
    for (size_t i = 0; i < joints.size(); i++) {
        Vec2f start, end;
        auto ctrls = get_curve_ctrl_point(i);
        std::cout << ctrls.first << "\n" << ctrls.second << "\n";
        start = joints[i];
        end = i == joints.size()-1 ? joints[0] : joints[i+1];
        for (size_t vertex = 0; vertex < vertices_per_curve; vertex++) {
            size_t v_index = i*vertices_per_curve + vertex;
            vArray[v_index].position = Helper::cubic_bezier_lerp(
                    start, end, ctrls.first, ctrls.second,
                    1.0f * vertex / (vertices_per_curve - 1));
            vArray[v_index].color = config.spline_color;
        }
    }
}
