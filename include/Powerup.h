//
// Created by Dorian on 09/05/2024.
//

#ifndef BREAKOUT_POWERUP_H
#define BREAKOUT_POWERUP_H

#include "types.h"
#include "config.h"
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
private:
    Vector2 velocity;
    float speed = POWERUP_SPEED;
protected:
    float radius = POWERUP_SIZE;
};

class multi_ball : public Powerup {
public:
    multi_ball(float x, float y, float vx, float vy) : Powerup(x, y, vx, vy) {}

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

    void spawnPowerup(float x, float y, float vx, float vy) {
        powerups.push_back(std::make_unique<multi_ball>(x, y, vx, vy));
    }

    void clearPowerups() {
        powerups.clear();
    }

private:
    std::vector<std::unique_ptr<Powerup>> powerups;
};

#endif //BREAKOUT_POWERUP_H
