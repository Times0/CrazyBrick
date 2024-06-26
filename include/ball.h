#ifndef CRAZYBRICK_BALL_H
#define CRAZYBRICK_BALL_H

#include <string>
#include <cmath>
#include "types.h"
#include "utils.h"
#include "config.h"

#ifdef _WIN32

#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif

class Ball {
public:
    Ball(double x, double y, double vx, double vy);

    void update(float dt);

    bool handleSolidCollision(const Polygon &polygon_points);

    void draw(SDL_Renderer *renderer) const;

    Vector2 center;
    Vector2 velocity;
    uint32_t radius;
    uint32_t speed;

    bool is_out_of_bounds() const;

private:
    void bounce(std::pair<Vector2, Vector2> mypair, float min_ditance);
};

#endif