#pragma once
#include <string>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

using Vec2f = sf::Vector2f;

template<typename T>
using Vec2 = sf::Vector2<T>;

namespace Helper {
    // get normal of a line from A to B
    template<typename T>
    inline Vec2f get_normal(const Vec2<T>& A, const Vec2<T>& B) {
        return (Vec2<T>) { B.y - A.y, -(B.x - A.x) };
    }

    template<typename T>
    inline Vec2<T> rotated(const Vec2<T>& v, float angle) {
        T rad = M_PI * angle / 180.0;
        T x = v.x, y = v.y;
        return Vec2<T>{
            std::cos(rad) * x + std::sin(rad) * y,
            -std::sin(rad) * x + std::cos(rad) * y
        };
    }

    template<typename T>
    inline Vec2<T> normalized(const Vec2<T>& v) {
        float len = std::sqrt(v.x * v.x + v.y * v.y);
        return v / len;
    }

    inline void draw_line(sf::RenderTarget& target, Vec2f A, Vec2f B, sf::Color c) {
        sf::Vertex v[] {
            sf::Vertex(A, c),
            sf::Vertex(B, c),
        };
        target.draw(v, 2, sf::Lines);
    }


    template<typename T>
    inline float distance(const Vec2<T>& v1, const Vec2<T>& v2) {
        return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
    }

    inline std::string input(const char* prompt) {
        std::string in;
        std::cout << prompt;
        fflush(stdout);
        std::getline(std::cin, in);
        fflush(stdin);
        return in;
    }

    template<typename T>
    inline Vec2<T> cubic_bezier_lerp(
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

    template<typename T>
    inline bool line_intersect(const Vec2<T>& X1, const Vec2<T>& X2,
        const Vec2<T>& X3, const Vec2<T>& X4) {
        double t = ((X1.x * 1.0 - X3.x) * (X3.y - X4.y) - (X1.y - X3.y) * (X3.x - X4.x)) /
            ((X1.x - X2.x) * (X3.y - X4.y) - (X1.y - X2.y) * (X3.x - X4.x));
        double u = ((X1.x * 1.0 - X3.x) * (X1.y - X2.y) - (X1.y - X3.y) * (X1.x - X2.x)) /
            ((X1.x - X2.x) * (X3.y - X4.y) - (X1.y - X2.y) * (X3.x - X4.x));
        return t <= 1 && t >= 0 && u <= 1 && u >= 0;
    }

    inline bool contains(const sf::ConvexShape& v, const Vec2f& position) {
        size_t n = v.getPointCount();
        Vec2f ref_point{position.x, -1};
        int count = 0;
        for (size_t i = 0; i < n - 1; i++) {
            if (line_intersect(v.getPoint(i), v.getPoint(i + 1), position, ref_point))
                count++;
        }
        if (line_intersect(v.getPoint(0), v.getPoint(n - 1), position, ref_point))
            count++;
        return count%2 == 1;
    }
};

inline std::istream& operator>>(std::istream& in, sf::Color& c) {
    int temp;
    in >> temp; c.r = sf::Uint8(temp);
    in >> temp; c.g = sf::Uint8(temp);
    in >> temp; c.b = sf::Uint8(temp);
    in >> temp; c.a = sf::Uint8(temp);
    return in;
}

inline std::ostream& operator<<(std::ostream& out, const sf::Color& c) {
    out << int(c.r) << " " << int(c.g) << " " << int(c.b) << " " << int(c.a);
    return out;
}

inline std::istream& operator>>(std::istream& in, sf::Vector2f& c) {
    in >> c.x >> c.y;
    return in;
}

inline std::ostream& operator<<(std::ostream& out, const sf::Vector2f& c) {
    out << c.x << " " << c.y;
    return out;
}
