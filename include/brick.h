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

class Brick {
public:
    Brick(Polygon polygon_points, int8_t type);

    ~Brick();

    void draw(SDL_Renderer *renderer) const;

    const Polygon &get_points() {
        return polygon_points;
    }

    Vector2 get_center();

    void decrease_collision_count();

    int8_t get_collision_count() const;


private:
    int8_t collisions_needed_to_destroy = 1;

    int8_t type;
    Polygon polygon_points;
    std::vector<SDL_Vertex> _vertices;
    std::vector<int32_t> _indices = {0, 1, 3, 1, 2, 3};

    std::vector<SDL_Vertex> get_vertices() const;
};


#endif //BREAKOUT_BRICK_H
