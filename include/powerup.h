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


#if defined(_WIN32)

#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif


class Powerup {
public:
    Powerup(double x, double y, double vx, double vy);

    void update(float dt);

    virtual void draw(SDL_Renderer *renderer) const;

    Vector2 center;
    float radius = POWERUP_SIZE;
private:
    Vector2 velocity;
    float speed = POWERUP_SPEED;
};

class MultiBall : public Powerup {
public:
    MultiBall(float x, float y, float vx, float vy) : Powerup(x, y, vx, vy) {}

    void draw(SDL_Renderer *renderer) const override;
};

class BiggerPaddle : public Powerup {
public:
    BiggerPaddle(float x, float y, float vx, float vy) : Powerup(x, y, vx, vy) {}

    void draw(SDL_Renderer *renderer) const override;
};

class PowerupManager {
public:
    PowerupManager() = default;

    ~PowerupManager() = default;

    void update(float dt) {
        // Update each power-up
        for (auto &powerup: powerups) {
            powerup->update(dt);
        }

        // Remove power-ups that are out of screen
        powerups.erase(std::remove_if(powerups.begin(), powerups.end(), [](const auto &powerup) {
            return powerup->center.y > GAME_HEIGHT;
        }), powerups.end());
    }

    void draw(SDL_Renderer *renderer) const {
        for (const auto &powerup: powerups) {
            powerup->draw(renderer);
        }
    }

    void spawnPowerup(float x, float y, float vx, float vy);

    void handlePaddleCollision(Polygon &paddle);

    void clearPowerups() {
        powerups.clear();
    }

private:
    std::vector<std::unique_ptr<Powerup>> powerups;
};

#endif //BREAKOUT_POWERUP_H
