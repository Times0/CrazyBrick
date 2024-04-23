#include "../include/ball.h"
#include "../include/config.h"
#include <cmath>

ball::ball(float x, float y, float vx, float vy) {
    center = {x, y};
    velocity = {vx, vy};
    speed = BALL_SPEED;
    radius = BALL_SIZE;
}

void ball::update(float dt) {
    center = {center.x + velocity.x * static_cast<float>(speed) * dt,
              center.y + velocity.y * static_cast<float>(speed) * dt};
}

void ball::draw(SDL_Renderer *renderer) const {
    // draw white ball
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180;
        SDL_RenderDrawPoint(renderer, static_cast<int>(center.x + radius * cos(angle)),
                            static_cast<int>(center.y + radius * sin(angle)));
    }
}

