#pragma once
#include <string>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

using Vec2f = sf::Vector2f;

template<typename T>
using Vec2 = sf::Vector2<T>;

class Helper {
public:

    // get normal of a line from A to B
    template<typename T>
    inline static Vec2f get_normal(const Vec2<T>& A, const Vec2<T>& B) {
        return (Vec2<T>) { B.y - A.y, -(B.x - A.x) };
    }

    template<typename T>
    inline static Vec2<T> rotated(const Vec2<T>& v, float angle) {
        T rad = M_PI * angle / 180.0;
        T x = v.x, y = v.y;
        return Vec2<T>{
            std::cos(rad) * x + std::sin(rad) * y,
            -std::sin(rad) * x + std::cos(rad) * y
        };
    }

    template<typename T>
    inline static Vec2<T> normalized(const Vec2<T>& v) {
        float len = std::sqrt(v.x * v.x + v.y * v.y);
        return v / len;
    }

    static void draw_line(sf::RenderTarget& target, Vec2f A, Vec2f B, sf::Color c) {
        sf::Vertex v[] {
            sf::Vertex(A, c),
            sf::Vertex(B, c),
        };
        target.draw(v, 2, sf::Lines);
    }


    template<typename T>
    static float distance(const Vec2<T>& v1, const Vec2<T>& v2) {
        return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
    }

    static std::string input(const char* prompt) {
        std::string in;
        std::cout << prompt;
        fflush(stdout);
        std::getline(std::cin, in);
        fflush(stdin);
        return in;
    }

    template<typename T>
    static Vec2<T> cubic_bezier_lerp(
            const Vec2<T>& start,
            const Vec2<T>& end,
            const Vec2<T>& start_ctrl,
            const Vec2<T>& end_ctrl,
            float t) {
        float a = (1 - t) * (1 - t) * (1 - t);
        float b = 3 * (1 - t) * (1 - t) * t;
        float c = 3 * (1 - t) * t * t;
        float d = t * t * t;

        float x = a * start.x + b * start_ctrl.x + c * end_ctrl.x + d * end.x;
        float y = a * start.y + b * start_ctrl.y + c * end_ctrl.y + d * end.y;
        return Vec2<T>{ x, y };
    }
};
