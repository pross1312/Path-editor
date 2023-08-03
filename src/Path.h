#pragma once
#include "Spline.h"


class Path: public sf::Drawable {
public:
	Path();
	~Path() = default;
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
	void on_user_editting(sf::Event& event, sf::RenderWindow& window);
	// void writeToFile(const char* fName);
	// void readFromFile(const char* fName);
	void update();


private:
	bool onEditing1 = true;
	Spline spline;
	sf::ConvexShape outer_shape, inner_shape;

};
