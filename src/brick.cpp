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
    switch (type) {
        case 0:
            collisions_needed_to_destroy = 1;
            break;
        case 1:
            collisions_needed_to_destroy = 2;
            break;
        case 2:
            collisions_needed_to_destroy = 3;
            break;
        case 3:
            collisions_needed_to_destroy = 4;
            break;
        default:
            break;
    }
}

Brick::~Brick() = default;

std::vector<SDL_Vertex> Brick::get_vertices() const {
    std::vector<SDL_Vertex> vertices;
    SDL_Color color;
    switch (collisions_needed_to_destroy) {
        case 0:
            color = {255, 228, 196, 255}; // Bisque
            break;
        case 1:
            color = {255, 182, 193, 255}; // Light Pink
            break;
        case 2:
            color = {216, 191, 216, 255}; // Thistle
            break;
        case 3:
            color = {176, 224, 230, 255}; // Powder Blue
            break;
        case 4:
            color = {255, 255, 0, 255}; // Pure Yellow
            break;
        default:
            color = {255, 255, 255, 255}; // White
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
    const auto vertices = get_vertices();
    SDL_RenderGeometry(renderer,
                       nullptr,
                       vertices.data(),
                       vertices.size(),
                       _indices.data(),
                       _indices.size());
}

Vector2 Brick::get_center() {
    Vector2 center = {0, 0};
    for (const auto &point: polygon_points) {
        center.x += point.x;
        center.y += point.y;
    }
    center.x /= polygon_points.size();
    center.y /= polygon_points.size();
    return center;
}


void Brick::decrease_collision_count() {
    collisions_needed_to_destroy--;
}


int8_t Brick::get_collision_count() const {
    return collisions_needed_to_destroy;
}

