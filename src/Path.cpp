#include "Path.h"
#include "Helper.h"

Path::Path()
    : _spline{}, pathShape1{0}, pathShape2{0} {
    pathShape1.setFillColor(Config::path_color);
    pathShape2.setFillColor(Config::back_ground);
    update();
}

void Path::onUserEditing(sf::Event& event, sf::RenderWindow& window) {
    _spline.onUserEditing(event, window);
    update();
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::R) {
            readFromFile(Helper::input("File path: ").c_str());
        }
        else if (event.key.code == sf::Keyboard::W)
            writeToFile(Helper::input("File path: ").c_str());
    }
}

void Path::writeToFile(const char* fName) {
    std::ofstream fout(fName);
    if (!fout.is_open())
        throw std::runtime_error("Can't open file to write.");
    _spline.writeToFile(fout);

    fout.close();
}

void Path::readFromFile(const char* fName) {
    std::ifstream fin(fName);
    if (!fin.is_open())
        throw std::runtime_error("Can't open file to read");

    _spline.readFromFile(fin);
    fin.close();
    update();
}



void Path::update() {
    const sf::VertexArray& vArray = _spline._splineArray;
    size_t n = vArray.getVertexCount();
    if (pathShape1.getPointCount() != n-2) {
        pathShape1.setPointCount(n-2);
        pathShape2.setPointCount(n-2);
    }
    int count = 0;
    for (size_t i = 1; i < n - 1; i++) {
        sf::Vector2f normal1 = Helper::getNormal(vArray[i-1].position, vArray[i].position);
        sf::Vector2f normal2 = Helper::getNormal(vArray[i].position, vArray[i+1].position);
        sf::Vector2f normal = Helper::addVector(normal1, normal2);
        Helper::normalize(normal);
        Helper::mul(normal, Config::path_width);
        pathShape1.setPoint(count, Helper::addVector(normal, vArray[i].position));
        Helper::mul(normal, -1);
        pathShape2.setPoint(count, Helper::addVector(normal, vArray[i].position));
        count++;
    }
}


void Path::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    (void)state;
    target.draw(pathShape1);
    target.draw(pathShape2);
    target.draw(_spline);    
}

