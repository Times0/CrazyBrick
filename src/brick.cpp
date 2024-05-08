//
// Created by Dorian on 08/05/2024.
//

#include "../include/brick.h"
#include <utility>
#include <vector>


brick::brick(Polygon polygon_points) : polygon_points(std::move(polygon_points)) {}

brick::~brick() = default;

void brick::draw(SDL_Renderer *renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (size_t i = 0; i < polygon_points.size(); i++) {
        SDL_RenderDrawLineF(renderer,
                            polygon_points[i].x,
                            polygon_points[i].y,
                            polygon_points[(i + 1) % polygon_points.size()].x,
                            polygon_points[(i + 1) % polygon_points.size()].y);
    }
}


