#include "Config.h"
#include <sstream>
#include <iostream>
#include <fstream>

std::istream& operator>>(std::istream& ss, sf::Color& c) {
    int temp;
    ss >> temp; c.r = sf::Uint8(temp);
    ss >> temp; c.g = sf::Uint8(temp);
    ss >> temp; c.b = sf::Uint8(temp);
    ss >> temp; c.a = sf::Uint8(temp);

    return ss;
}

std::ostream& operator<<(std::ostream& fout, const sf::Color& c) {
    fout << int(c.r) << " " << int(c.g) << " " << int(c.b) << " " << int(c.a);
    return fout;
}

void Config::read_config(const char* filePath) {
    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        printf("Can't open file %s to read config.", filePath);
        exit(1);
    }
    std::stringstream ss;
    std::string s;
    while (true) {
        std::getline(fin, s);
        if (s.size() < 2)
            break;
        std::string sub = s.substr(s.find(':', 0)+1);
        ss << sub;
    }
    ss >> back_ground >> path_color >> path_width >> nJoints >> spline_color >> joint_color >> ctrl_point_color
       >> line_color >> joint_radius >> ctrl_point_size >> curve_color >> vertexes_per_curve;
    fin.close();
}


void Config::print_config() {
    std::cout << "back ground color: " << back_ground << "\n";
    std::cout << "path color: " << path_color << "\n";
    std::cout << "path width: " << path_width << "\n";
    std::cout << "number of curves: " << nJoints << "\n";
    std::cout << "spline color: " << spline_color << "\n";
    std::cout << "joint color: " << joint_color << "\n";
    std::cout << "ctrl_point color: " << ctrl_point_color << "\n";
    std::cout << "line color: " << line_color << "\n";
    std::cout << "joint radius: " << joint_radius << "\n";
    std::cout << "ctrl_point size: " << ctrl_point_size << "\n";
    std::cout << "curve color: " << curve_color << "\n";
    std::cout << "number of vertexs: " << vertexes_per_curve << "\n";
}


void Config::write_config(const char* filePath) {
    std::ofstream fout(filePath, std::ofstream::openmode::_S_out);
    if (!fout.is_open()) {
        printf("Can't open file %s to write config.", filePath);
        exit(1);
    }
    fout << "back ground color: " << back_ground << "\n";

    fout << "path color: " << path_color << "\n";
    fout << "path width: " << path_width << "\n";
    fout << "number of curves: " << nJoints << "\n";
    fout << "spline color: " << spline_color << "\n";
    fout << "joint color: " << joint_color << "\n";
    fout << "ctrl_point color: " << ctrl_point_color << "\n";
    fout << "line color: " << line_color << "\n";
    fout << "joint radius: " << joint_radius << "\n";
    fout << "ctrl_point size: " << ctrl_point_size << "\n";
    fout << "curve color: " << curve_color << "\n";
    fout << "number of vertexs: " << vertexes_per_curve << "\n";

    fout.close();
}
