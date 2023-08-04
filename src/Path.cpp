#include "Path.h"
#include "Helper.h"
#include <algorithm>

Path::Path():
    spline{},
    outer_shape{ spline.vArray.getVertexCount() },
    inner_shape{ spline.vArray.getVertexCount() } {
        outer_shape.setFillColor(Config::path_color);
        inner_shape.setFillColor(Config::back_ground);
        update();
}
void Path::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    target.draw(outer_shape, state);
    target.draw(inner_shape, state);
    target.draw(spline, state);
}

void Path::update() {
    size_t nVertices = spline.vArray.getVertexCount();
    for (size_t i = 0; i < nVertices; i++) {
        Vec2f left_vertex  = i == 0           ? spline.vArray[nVertices-1].position : spline.vArray[i-1].position;
        Vec2f right_vertex = i == nVertices-1 ? spline.vArray[0].position           : spline.vArray[i+1].position;
        Vec2f cur_vertex   = spline.vArray[i].position;
        Vec2f left_normal  = Helper::get_normal(left_vertex, cur_vertex);
        Vec2f right_normal = Helper::get_normal(cur_vertex, right_vertex);
        Vec2f normal       = Helper::normalized(left_normal + right_normal);
        outer_shape.setPoint(i, cur_vertex + normal*Config::path_width);
        inner_shape.setPoint(i, cur_vertex - normal*Config::path_width);
    }
}

// void Path::writeToFile(const char* fName) {
//     std::ofstream fout(fName);
//     if (!fout.is_open())
//         throw std::runtime_error("Can't open file to write.");
//     _spline.writeToFile(fout);
//     fout.close();
// }

// void Path::readFromFile(const char* fName) {
//     std::ifstream fin(fName);
//     if (!fin.is_open())
//         throw std::runtime_error("Can't open file to read");
//
//     _spline.readFromFile(fin);
//     fin.close();
//     update();
// }
