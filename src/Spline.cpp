#include "Spline.h"
#include "Helper.h"
#include <exception>
#include <iostream>
#define PTR std::make_shared
using Vec2f = sf::Vector2f;

Spline::Spline():
    joints      { Config::nJoints   }, // start - end - start - end - ...
    joint_ctrls { Config::nJoints*2 },
    vArray      { sf::LineStrip, Config::nJoints * Config::vertexes_per_curve } {
    size_t w = Config::screen_w;
    size_t h = Config::screen_h;
    float angle = M_PI;
    float angle_step = 2.0*M_PI / Config::nJoints;
    float radius = 400;
    float dis = 80;
    for (size_t i = 0; i < Config::nJoints; i++) {
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
    for (const auto& joint_ctrl : joint_ctrls) {
        Vec2f size{ Config::ctrl_point_size, Config::ctrl_point_size };
        sf::RectangleShape r(size);
        r.setOrigin(size / 2.0f);
        r.setPosition(joint_ctrl);
        target.draw(r);
    }
    for (size_t i = 0; i < joints.size(); i++) {
        sf::CircleShape c(Config::joint_radius);
        c.setOrigin(Config::joint_radius, Config::joint_radius);
        c.setPosition(joints[i]);
        c.setFillColor(Config::joint_color);
        target.draw(c, state);
        Helper::draw_line(target, joints[i], joint_ctrls[2*i]  , Config::line_color);
        Helper::draw_line(target, joints[i], joint_ctrls[2*i+1], Config::line_color);
    }
}

void Spline::update() {
    for (size_t i = 0; i < Config::nJoints; i++) {
        Vec2f start, end;
        auto ctrls = get_curve_ctrl_point(i);
        start = joints[i];
        end = i == Config::nJoints-1 ? joints[0] : joints[i+1];
        for (size_t vertex = 0; vertex < Config::vertexes_per_curve; vertex++) {
            vArray[i*Config::vertexes_per_curve + vertex].position = Helper::cubic_bezier_lerp(
                    start, end, ctrls.first, ctrls.second,
                    1.0f * vertex / (Config::vertexes_per_curve - 1));
        }
    }
}

// void Spline::writeToFile(std::ofstream& fout) {
//     fout << _curves.size() << "\n";
//     for (CubicCurve& c : _curves)
//         c.writeToFile(fout);
// }

// void Spline::readFromFile(std::ifstream& fin) {
//     // change number of curves in config and regenerate spline
//     fin >> Config::nJoints;
//     if (_curves.size() == Config::nJoints) {
//         for (unsigned i = 0; i < Config::nJoints; i++)
//             _curves[i].readFromFile(fin);
//     }
//     else {
//         Spline temp{};
//         *this = temp;
//         for (unsigned i = 0; i < Config::nJoints; i++)
//             _curves[i].readFromFile(fin);
//     }
//     update();
// }
