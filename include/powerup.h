//
// Created by Dorian on 09/05/2024.
//

#ifndef BREAKOUT_POWERUP_H
#define BREAKOUT_POWERUP_H

#include "types.h"
#include "config.h"
#include "utils.h"
#include "ball.h"
#include <cmath>
#include <memory>
#include <algorithm>
#include <list>

#if defined(_WIN32)

#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif

class Game;

class Powerup {
public:
    Powerup(double x, double y, double vx, double vy);

    virtual std::unique_ptr<Powerup> create(float x, float y, float vx, float vy) const = 0;

    void update(float dt);

    virtual void draw(SDL_Renderer *renderer) const;

    Vector2 center;
    float radius = POWERUP_SIZE;

    virtual void apply_effect(Game *game) = 0;

private:

    Vector2 velocity;
    float speed = POWERUP_SPEED;
};

class MultiBall : public Powerup {
public:
    MultiBall(float x, float y, float vx, float vy) : Powerup(x, y, vx, vy) {}

    std::unique_ptr<Powerup> create(float x, float y, float vx, float vy) const override {
        return std::make_unique<MultiBall>(x, y, vx, vy);
    }

    void apply_effect(Game *game) override;

    void draw(SDL_Renderer *renderer) const override;
};

class BiggerPaddle : public Powerup {
public:
    BiggerPaddle(float x, float y, float vx, float vy) : Powerup(x, y, vx, vy) {}

    std::unique_ptr<Powerup> create(float x, float y, float vx, float vy) const override {
        return std::make_unique<BiggerPaddle>(x, y, vx, vy);
    }

    void apply_effect(Game *game) override;

    void draw(SDL_Renderer *renderer) const override;
};

class DoubleBalls : public Powerup {
public:
    DoubleBalls(float x, float y, float vx, float vy) : Powerup(x, y, vx, vy) {}

    std::unique_ptr<Powerup> create(float x, float y, float vx, float vy) const override {
        return std::make_unique<DoubleBalls>(x, y, vx, vy);
    }

    void apply_effect(Game *game) override;

    void draw(SDL_Renderer *renderer) const override;
};

class PowerupManager {
public:
    PowerupManager() = default;

    ~PowerupManager() = default;

    void update(float dt) {
        for (auto &powerup: powerups) {
            powerup->update(dt);
        }

        // Remove power-ups that are out of screen (all directions) using the fact that it is a list
        powerups.remove_if([](const std::unique_ptr<Powerup> &powerup) {
            return powerup->center.y - powerup->radius > GAME_HEIGHT;
        });
    }

    void draw(SDL_Renderer *renderer) const {
        for (const auto &powerup: powerups) {
            powerup->draw(renderer);
        }
    }

    void spawn_random_powerup(float x, float y, float vx, float vy);

    static std::unique_ptr<Powerup> create_powerup(const std::string &name, float x, float y, float vx, float vy);

    void handle_collision_with_paddle(const Polygon &paddle);

    void bind(Game *game) {
        game_ptr = game;
    }

private:
    std::list<std::unique_ptr<Powerup>> powerups;
    Game *game_ptr{};
};

#endif //BREAKOUT_POWERUP_H
