//
// Created by Dorian on 09/05/2024.
//

#include <cmath>
#include <memory>

#include "../include/powerup.h"
#include "../include/game.h"

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

void MultiBall::apply_effect(Game *game) {
    game->add_ball();
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

void BiggerPaddle::apply_effect(Game *game) {
    game->increase_paddle_size();
}

void DoubleBalls::draw(SDL_Renderer *renderer) const {
    Powerup::draw(renderer);

    // set color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // draw a square
    SDL_Rect rect = {static_cast<int>(center.x - radius / 2), static_cast<int>(center.y - radius / 2),
                     static_cast<int>(radius), static_cast<int>(radius)};
    SDL_RenderFillRect(renderer, &rect);
}

void DoubleBalls::apply_effect(Game *game) {
    game->double_balls();
}

void FasterBall::apply_effect(Game *game) {
    game->increase_ball_speed();
}

void FasterBall::draw(SDL_Renderer *renderer) const {
    Powerup::draw(renderer);

    // set color to red
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // draw a square
    SDL_Rect rect = {static_cast<int>(center.x - radius / 2), static_cast<int>(center.y - radius / 2),
                     static_cast<int>(radius), static_cast<int>(radius)};
    SDL_RenderFillRect(renderer, &rect);
}


// powerup_manager class implementation
void PowerupManager::spawn_random_powerup(float x, float y, float vx, float vy) {
    static std::vector<std::string> powerupTypes = {"MultiBall", "BiggerPaddle", "DoubleBalls", "FasterBall"};
    std::string powerupType = powerupTypes[random_int(0, powerupTypes.size() - 1)];
    auto powerup = create_powerup(powerupType, x, y, vx, vy);
    if (powerup) {
        powerups.push_back(std::move(powerup));
    }
}

void PowerupManager::handle_collision_with_paddle(const Polygon &paddle) {
    powerups.remove_if([&paddle, this](const std::unique_ptr<Powerup> &powerup) {
        if (handlePolygonCircleCollision(paddle, powerup->center, powerup->radius)) {
            powerup->apply_effect(game_ptr);
            return true;
        }
        return false;
    });
}

std::unique_ptr<Powerup> PowerupManager::create_powerup(const std::string &name, float x, float y, float vx, float vy) {
    if (name == "MultiBall") {
        return std::make_unique<MultiBall>(x, y, vx, vy);
    } else if (name == "BiggerPaddle") {
        return std::make_unique<BiggerPaddle>(x, y, vx, vy);
    } else if (name == "DoubleBalls") {
        return std::make_unique<DoubleBalls>(x, y, vx, vy);
    } else if (name == "FasterBall") {
        return std::make_unique<FasterBall>(x, y, vx, vy);
    }
    return nullptr;
}
