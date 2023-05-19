#pragma once
#include "CubicCurve.h"
#include "Config.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Spline: public sf::Drawable {
public:
	Spline();
	~Spline() = default;

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
	void onUserEditing(sf::Event& event, sf::RenderWindow& window);
	void writeToFile(std::ofstream& fout);
	void readFromFile(std::ifstream& fin);
	void update();
	friend class Path;
private:
	std::shared_ptr<sf::RectangleShape>& getCtrlPoint(unsigned index);
private:
	enum State {
		Idle,
		JointEditing,
		ControlJointEditing,
	};

	State current = State::Idle;
	unsigned indexEditing = 0;
	unsigned nControlPoints = 0;
	std::vector<CubicCurve> _curves;
	sf::VertexArray _splineArray;
};
