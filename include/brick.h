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

    [[nodiscard]] const Polygon &getPoints() const {
        return polygon_points;
    }

    Vector2 getCenter();

private:
    int8_t type;
    Polygon polygon_points;
};


#endif //BREAKOUT_BRICK_H
