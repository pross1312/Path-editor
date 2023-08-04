#pragma once
#include "Config.h"
#include "Helper.h"
#include <vector>
#include <assert.h>
#include <memory>
#include <SFML/Graphics.hpp>

class Spline: public sf::Drawable {
public:
	Spline();
	~Spline() = default;
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
	void update();

    inline std::pair<Vec2f, Vec2f> get_curve_ctrl_point(size_t i) const {
        assert(i < joints.size());
        if (i != joints.size()-1)
            return std::make_pair( joint_ctrls[2*i + 1], joint_ctrls[2*(i+1)] );
        return std::make_pair( joint_ctrls[2*i + 1], joint_ctrls[0] );
    }

    std::vector<Vec2f> joints, joint_ctrls;
    sf::VertexArray vArray;
};
