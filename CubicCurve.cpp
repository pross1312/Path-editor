#include "CubicCurve.h"

sf::Vector2f CubicCurve::cubicBezierLerp(float t) const {
    float a = (1 - t) * (1 - t) * (1 - t);
    float b = 3 * (1 - t) * (1 - t) * t;
    float c = 3 * (1 - t) * t * t;
    float d = t * t * t;

    sf::Vector2f _start{ start->getPosition() };
    sf::Vector2f _startCtrl{ startCtrl->getPosition() };
    sf::Vector2f _dest{ end->getPosition() };
    sf::Vector2f _destCtrl{ endCtrl->getPosition() };

    float x = a * _start.x + b * _startCtrl.x + c * _destCtrl.x + d * _dest.x;
    float y = a * _start.y + b * _startCtrl.y + c * _destCtrl.y + d * _dest.y;
    return (sf::Vector2f) { x, y };
}

void CubicCurve::writeToFile(std::ofstream& fout) {
    fout << vArray.getVertexCount() << "\n";
    fout << start->getPosition().x << " " << start->getPosition().y << " ";
    fout << startCtrl->getPosition().x << " " << startCtrl->getPosition().y << " ";
    fout << endCtrl->getPosition().x << " " << endCtrl->getPosition().y << " ";
    fout << end->getPosition().x << " " << end->getPosition().y << "\n";
}
void CubicCurve::readFromFile(std::ifstream& fin) {
    sf::Vector2f startPos, startCtrlPos, endCtrlPos, endPos;
    unsigned nVertex = 0;
    fin >> nVertex;
    if (vArray.getVertexCount() != nVertex) {
        vArray.resize(nVertex);
        Config::nVertexs = nVertex;
    }
    fin >> startPos.x  >> startPos.y;
    fin >> startCtrlPos.x  >> startCtrlPos.y;
    fin >> endCtrlPos.x  >> endCtrlPos.y;
    fin >> endPos.x  >> endPos.y;
    start->setPosition(startPos);
    startCtrl->setPosition(startCtrlPos);
    endCtrl->setPosition(endCtrlPos);
    end->setPosition(endPos);
    update();
}

void CubicCurve::update() {
    for (int i = 0; i < Config::nVertexs; i++) {
        vArray[i].position = this->cubicBezierLerp(1.0f * i / (Config::nVertexs - 1));
        vArray[i].color = Config::curve_color;
    }
}

void CubicCurve::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    target.draw(vArray);
}