#include "Spline.h"
#include "Helper.h"
#include <exception>
#include <iostream>
Spline::Spline(): _curves{ Config::nCurves }, _splineArray{ sf::LineStrip, 0 } {
    if (Config::nCurves < 1)
        throw std::invalid_argument("Number of vertex can't be less than 2.");
    nControlPoints = 2 * Config::nCurves;
    for (unsigned i = 0; i < Config::nCurves; i++) {
        if (i > 0) {
            _curves[i].start = _curves[i - 1].end;
        }
        else {
            _curves[i].start = std::make_shared<sf::CircleShape>(Config::jointRadius);
            _curves[i].start->setOrigin(Config::jointRadius, Config::jointRadius);
            _curves[i].start->setFillColor(Config::joint_color);
            _curves[i].start->setPosition((i + 1) * 100, (i + 1) * 100);
        }
        if (i == Config::nCurves - 1) {
            _curves[i].end = _curves[0].start;
        }
        else {
            _curves[i].end = std::make_shared<sf::CircleShape>(Config::jointRadius);
            _curves[i].end->setOrigin(Config::jointRadius, Config::jointRadius);
            _curves[i].end->setFillColor(Config::joint_color);
            _curves[i].end->setPosition((i + 1) * 100 + 60, (i + 1) * 100 + 60);

        }

        _curves[i].startCtrl = std::make_shared<sf::RectangleShape>((sf::Vector2f) { Config::ctrlPointSize, Config::ctrlPointSize });
        _curves[i].endCtrl = std::make_shared<sf::RectangleShape>((sf::Vector2f) { Config::ctrlPointSize, Config::ctrlPointSize });


        _curves[i].startCtrl->setOrigin(Config::ctrlPointSize / 2.0f, Config::ctrlPointSize / 2.0f);
        _curves[i].startCtrl->setFillColor(Config::ctrl_point_color);
        _curves[i].startCtrl->setPosition((i + 1) * 100 + 20, (i + 1) * 100 + 20);

        _curves[i].endCtrl->setOrigin(Config::ctrlPointSize / 2.0f, Config::ctrlPointSize / 2.0f);
        _curves[i].endCtrl->setFillColor(Config::ctrl_point_color);
        _curves[i].endCtrl->setPosition((i + 1) * 100 + 40, (i + 1) * 100 + 40);
        _curves[i].update();
    }
    update();
}

std::shared_ptr<sf::RectangleShape>& Spline::getCtrlPoint(unsigned index) {
    if (index % 2 == 0)
        return _curves[index / 2].startCtrl;
    return _curves[(index - 1) / 2].endCtrl;
}

void Spline::onUserEditing(sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (current == State::Idle) {
            for (size_t i = 0; i < _curves.size(); i++) {
                sf::CircleShape& joint = *_curves[i].start;
                if (joint.getGlobalBounds().contains(mousePos)) {
                    indexEditing = i;
                    current = State::JointEditing;
                    break;
                }
            }
            for (size_t i = 0; i < nControlPoints; i++) {
                sf::RectangleShape& ctrlPoint = *getCtrlPoint(i);
                if (ctrlPoint.getGlobalBounds().contains(mousePos)) {
                    indexEditing = i;
                    current = State::ControlJointEditing;
                    break;
                }
            }
        }
        else
            current = State::Idle;
    }
    else if (current == State::JointEditing) {
//    else if (current == State::JointEditing) {
        sf::CircleShape& joint = *_curves[indexEditing].start;
        joint.setPosition(mousePos);
        update();
    }
    else if (current == State::ControlJointEditing) {
        sf::RectangleShape& ctrlPoint = *getCtrlPoint(indexEditing);
        ctrlPoint.setPosition(mousePos);
        update();
    }
}

void Spline::update() {

    int n = Config::nVertexs;

    if (_splineArray.getVertexCount() != (n - 1) * _curves.size() + 2)
        _splineArray.resize((n - 1) * _curves.size() + 2);

    for (size_t j = 0; j < _curves.size(); j++) {
        _curves[j].update();
        const sf::VertexArray& vArray = _curves[j].vArray;
        for (int i = 0; i < n - 1; i++)
            _splineArray[(j * (n - 1)) + i + 1] = vArray[i];
    }
    _splineArray[0] = (*--_curves.end()).vArray[n - 1];
    _splineArray[_splineArray.getVertexCount() - 1] = _curves[0].vArray[0];
}



void Spline::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    (void)state;
    sf::VertexArray line{ sf::Lines, 2 };
    target.draw(_splineArray);
    for (size_t i = 0; i < _curves.size(); i++) {
        Helper::drawLine(target, _curves[i].start->getPosition(), _curves[i].startCtrl->getPosition(), Config::line_color);
        Helper::drawLine(target, _curves[i].end->getPosition(), _curves[i].endCtrl->getPosition(), Config::line_color);
        target.draw(*_curves[i].start);
        target.draw(*_curves[i].startCtrl);
        target.draw(*_curves[i].endCtrl);
    }
}

void Spline::writeToFile(std::ofstream& fout) {
    fout << _curves.size() << "\n";
    for (CubicCurve& c : _curves)
        c.writeToFile(fout);
}

void Spline::readFromFile(std::ifstream& fin) {
    // change number of curves in config and regenerate spline
    fin >> Config::nCurves;
    if (_curves.size() == Config::nCurves) {
        for (unsigned i = 0; i < Config::nCurves; i++)
            _curves[i].readFromFile(fin);
    }
    else {
        Spline temp{};
        *this = temp;
        for (unsigned i = 0; i < Config::nCurves; i++)
            _curves[i].readFromFile(fin);
    }
    update();
}
