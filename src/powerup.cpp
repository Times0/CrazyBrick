//
// Created by Dorian on 09/05/2024.
//

#include <cmath>
#include <memory>

#include "../include/powerup.h"

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

// MultiBall class implementation
void MultiBall::draw(SDL_Renderer *renderer) const {
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

void BiggerPaddle::draw(SDL_Renderer *renderer) const {
    Powerup::draw(renderer);

    // set color to yellow
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    // draw a square
    SDL_Rect rect = {static_cast<int>(center.x - radius / 2), static_cast<int>(center.y - radius / 2),
                     static_cast<int>(radius), static_cast<int>(radius)};
    SDL_RenderFillRect(renderer, &rect);
}


// powerup_manager class implementation
void PowerupManager::spawnPowerup(float x, float y, float vx, float vy) {
    int powerupType = myRandomInt(0, 1);
    if (powerupType == 0) {
        powerups.push_back(std::make_unique<MultiBall>(x, y, vx, vy));
    } else {
        powerups.push_back(std::make_unique<BiggerPaddle>(x, y, vx, vy));
    }

}

void PowerupManager::handlePaddleCollision(Polygon &paddle) {
    powerups.erase(std::remove_if(powerups.begin(), powerups.end(), [&paddle](const auto &powerup) {
        if (handlePolygonCircleCollision(paddle, powerup->center, powerup->radius)) {
            // Apply power-up effect


            return true;
        }
        return false;
    }), powerups.end());
}