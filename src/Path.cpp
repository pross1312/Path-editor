#include "Path.h"
#include "Helper.h"
#include <fstream>

#define N_JOINT 10
#define N_VERTICES_PER_CURVE 25
#define SPLINE_COLOR 0x5F9394ff

Path::Path(float w, uint32_t c):
    width       { w },
    color       { c },
    spline      { N_JOINT, N_VERTICES_PER_CURVE, SPLINE_COLOR},
    vArray      { sf::TriangleStrip, spline.vArray.getVertexCount()*2+2 }{
        update();
}

void Path::update() {
    spline.update();
    size_t nVertices = spline.vArray.getVertexCount();
    if (vArray.getVertexCount() != 2*nVertices+2) vArray.resize(2*nVertices+2);
    for (size_t i = 0; i < nVertices; i++) {
        Vec2f left_vertex  = i == 0           ? spline.vArray[nVertices-1].position : spline.vArray[i-1].position;
        Vec2f right_vertex = i == nVertices-1 ? spline.vArray[0].position           : spline.vArray[i+1].position;
        Vec2f cur_vertex   = spline.vArray[i].position;
        Vec2f left_normal  = Helper::get_normal(left_vertex, cur_vertex);
        Vec2f right_normal = Helper::get_normal(cur_vertex, right_vertex);
        Vec2f normal       = Helper::normalized(left_normal + right_normal);
        vArray[2*i].position   = cur_vertex + normal*width;
        vArray[2*i+1].position = cur_vertex - normal*width;
        vArray[2*i].color      = vArray[2*i+1].color = sf::Color(color);
    }
    vArray[2*nVertices].position   = vArray[0].position;
    vArray[2*nVertices+1].position = vArray[1].position;
    vArray[2*nVertices].color      = vArray[2*nVertices+1].color = sf::Color(color);
}

bool Path::save(const char* fName) {
    std::ofstream fout(fName, std::ios::binary | std::ios::out);
    if (!fout.is_open()) {
        printf("Can't open file %s\n", fName);
        return false;
    }
    fout.write((char*)&width, sizeof(width));
    fout.write((char*)&color, sizeof(color));
    fout << spline;
    fout.close();
    return true;
}

bool Path::load(const char* fName) {
    std::ifstream fin(fName, std::ios::binary | std::ios::in);
    if (!fin.is_open()) {
        printf("Can't open file %s\n", fName);
        return false;
    }
    fin.read((char*)&width, sizeof(width));
    fin.read((char*)&color, sizeof(color));
    fin >> spline;
    fin.close();
    update();
    return true;
}

void Path::zoom(Vec2f center, float ratio) {
    width *= ratio;
    for (auto& joint : spline.joints) {
        joint = center + (joint - center) * ratio;
    }
    for (auto& joint_ctrl : spline.joint_ctrls) {
        joint_ctrl = center + (joint_ctrl - center)*ratio;
    }
    update();
}
