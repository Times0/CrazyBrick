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
    brick(Polygon polygon_points);

    ~brick();

    void draw(SDL_Renderer *renderer) const;

private:
    Polygon polygon_points;
};


#endif //BREAKOUT_BRICK_H
