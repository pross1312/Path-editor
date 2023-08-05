#pragma once
#include "Spline.h"


class Path: public sf::Drawable {
public:
	Path();
	~Path() = default;
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
	void save(const char* fName);
	void load(const char* fName);
	void update();

	Spline spline;
	sf::ConvexShape outer_shape, inner_shape;
};
