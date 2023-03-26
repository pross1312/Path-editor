#pragma once
#include <string>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

class Helper {
public:
    template<typename T>
    inline static sf::Vector2f getNormal(const sf::Vector2<T>& A, const sf::Vector2<T>& B) {
        return (sf::Vector2<T>) { B.y - A.y, -(B.x - A.x) };
    }

    template<typename T>
    inline static void rotate(sf::Vector2<T>& v, float angle) {
        T rad = M_PI * angle / 180.0;
        T x = v.x, y = v.y;
        v = (sf::Vector2<T>){
            std::cos(rad) * x + std::sin(rad) * y,
            -std::sin(rad) * x + std::cos(rad) * y
        };
    }

    template<typename T>
    inline static void mul(sf::Vector2<T>& v, const float& scalar) {
        v.x *= (T)scalar;
        v.y *= (T)scalar;
    }

    template<typename T>
    inline static sf::Vector2<T> addVector(const sf::Vector2<T>& A, const sf::Vector2<T>& B) {
        return (sf::Vector2<T>) { A.x + B.x, A.y + B.y };
    }

    template<typename T>
    inline static void normalize(sf::Vector2<T>& v) {
        float len = std::sqrt(v.x * v.x + v.y * v.y);
        v.x /= len;
        v.y /= len;
    }

    

    static void drawLine(sf::RenderTarget& target, sf::Vector2f A, sf::Vector2f B, sf::Color c) {
        sf::Vertex v[]{
            (sf::Vertex) {A, c},
            (sf::Vertex) {B, c},
        };
        target.draw(v, 2, sf::Lines);
    }


    template<typename T>
    static float distance(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) {
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
};