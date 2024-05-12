//
// Created by Dorian on 08/05/2024.
//

#include "../include/brick.h"
#include <utility>
#include <vector>
#include <cmath>


brick::brick(Polygon polygon_points, int8_t type) : polygon_points(std::move(polygon_points)), type(type) {
    _vertices.reserve(polygon_points.size());
}

brick::~brick() = default;

std::vector<SDL_Vertex> brick::getVertices() const {
    std::vector<SDL_Vertex> vertices;
    SDL_Color color;
    switch (type) {
        case 0:
            color = {255, 0, 0, 255}; // Red
            break;
        case 1:
            color = {0, 255, 0, 255}; // Green
            break;
        case 2:
            color = {0, 0, 255, 255}; // Blue
            break;
        case 3:
            color = {255, 255, 0, 255}; // Yellow
            break;
        default:
            color = {255, 50, 255, 255}; // Default color (purple)
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

void brick::draw(SDL_Renderer *renderer) const {
    const auto vertices = getVertices();
    SDL_RenderGeometry(renderer,
                       nullptr,
                       vertices.data(),
                       vertices.size(),
                       _indices.data(),
                       _indices.size());
}

Vector2 brick::getCenter() {
    Vector2 center = {0, 0};
    for (const auto &point: polygon_points) {
        center.x += point.x;
        center.y += point.y;
    }
    center.x /= polygon_points.size();
    center.y /= polygon_points.size();
    return center;
}



