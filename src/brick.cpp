//
// Created by Dorian on 08/05/2024.
//

#include "../include/brick.h"
#include <utility>
#include <vector>


brick::brick(Polygon polygon_points, int8_t type) : polygon_points(std::move(polygon_points)), type(type) {}

brick::~brick() = default;

void brick::draw(SDL_Renderer *renderer) const {
    // type 0 is white 1 is red 2 is green 3 is blue

    switch (type) {
        case 0:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            break;
        case 1:
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            break;
        case 2:
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            break;
        case 3:
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            break;

    }

    for (size_t i = 0; i < polygon_points.size(); i++) {
        SDL_RenderDrawLineF(renderer,
                            polygon_points[i].x,
                            polygon_points[i].y,
                            polygon_points[(i + 1) % polygon_points.size()].x,
                            polygon_points[(i + 1) % polygon_points.size()].y);
    }
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



