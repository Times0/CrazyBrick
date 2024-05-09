//
// Created by Dorian on 09/05/2024.
//

#include <cmath>
#include <memory>

#include "../include/Powerup.h"

// Powerup class implementation
Powerup::Powerup(double x, double y, double vx, double vy) : center({x, y}), velocity({vx, vy}) {}

void Powerup::update(float dt) {
    center = {center.x + velocity.x * speed * dt,
              center.y + velocity.y * speed * dt};
}

void Powerup::draw(SDL_Renderer *renderer) const {
    // draw white circle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180;
        SDL_RenderDrawPoint(renderer, static_cast<int>(center.x + radius * std::cos(angle)),
                            static_cast<int>(center.y + radius * std::sin(angle)));
    }
}

// multi_ball class implementation
void multi_ball::draw(SDL_Renderer *renderer) const {
    Powerup::draw(renderer);

    // set color to pink
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);

    // draw a smaller circle
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180;
        SDL_RenderDrawPoint(renderer, static_cast<int>(center.x + radius / 2 * std::cos(angle)),
                            static_cast<int>(center.y + radius / 2 * std::sin(angle)));
    }

}

// powerup_manager class implementation

