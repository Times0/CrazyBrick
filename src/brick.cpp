//
// Created by Dorian on 08/05/2024.
//

#include "../include/brick.h"
#include <utility>
#include <vector>
#include <cmath>


Brick::Brick(Polygon polygon_points, int8_t type) : polygon_points(std::move(polygon_points)), type(type) {
    _vertices.reserve(polygon_points.size());
    //we initialize the collision count according to the type of the brick
    switch(type){
        case 0:
            collision_count = 1;
            break;
        case 1:
            collision_count = 2;
            break;
        case 2:
            collision_count = 3;
            break;
        case 3:
            collision_count = 4;
            break;
        default:
            collision_count = 1;
            break;
    }
}

Brick::~Brick() = default;

std::vector<SDL_Vertex> Brick::getVertices() const {
    std::vector<SDL_Vertex> vertices;
    SDL_Color color;
    switch (type) {
        case 0:
            color = {156, 39, 176, 255}; // Tomato
            break;
        case 1:
            color = {76, 175, 80, 255}; // Green Sea
            break;
        case 2:
            color = {33, 150, 243, 255}; // Blue Sky
            break;
        case 3:
            color = {255, 235, 59, 255}; // Yellow Sun
            break;
        default:
            color = {156, 39, 176, 255}; // Purple Plum
            break;
    }

    for (int i = 0; i < polygon_points.size(); i++) {
        const auto &point = polygon_points[i];
        vertices.push_back(SDL_Vertex{SDL_FPoint{static_cast<float>(point.x), static_cast<float>(point.y)},
                                      SDL_Color{static_cast<Uint8>(color.r * std::pow(0.7, i)),
                                                color.g,
                                                static_cast<Uint8>(color.b * std::pow(0.7, i)),
                                                color.a}});
    }
    return vertices;
}

void Brick::draw(SDL_Renderer *renderer) const {
    const auto vertices = getVertices();
    SDL_RenderGeometry(renderer,
                       nullptr,
                       vertices.data(),
                       vertices.size(),
                       _indices.data(),
                       _indices.size());
}

Vector2 Brick::getCenter() {
    Vector2 center = {0, 0};
    for (const auto &point: polygon_points) {
        center.x += point.x;
        center.y += point.y;
    }
    center.x /= polygon_points.size();
    center.y /= polygon_points.size();
    return center;
}


void Brick::decreaseCollisionCount() {
    collision_count--;
}


int8_t Brick::getCollisionCount() const {
    return collision_count;
}

