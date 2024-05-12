//
// Created by Dorian on 08/05/2024.
//

#ifndef BREAKOUT_BRICK_H
#define BREAKOUT_BRICK_H

#include "types.h"

#include <vector>

#ifdef _WIN32

#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif

class brick {
public:
    brick(Polygon polygon_points, int8_t type);

    ~brick();

    void draw(SDL_Renderer *renderer) const;

    const Polygon &getPoints() {
        return polygon_points;
    }

    Vector2 getCenter();

private:
    int8_t type;
    Polygon polygon_points;

    std::vector<SDL_Vertex> _vertices;
    std::vector<int32_t> _indices = {0, 1, 3, 1, 2, 3};

    std::vector<SDL_Vertex> getVertices() const;
};


#endif //BREAKOUT_BRICK_H
