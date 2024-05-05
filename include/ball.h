#ifndef CRAZYBRICK_BALL_H
#define CRAZYBRICK_BALL_H

#include <string>

#ifdef _WIN32

#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif

class ball {
public:
    ball(float x, float y, float vx, float vy);

    void update(float dt);

    void draw(SDL_Renderer *renderer) const;

    // getters
    SDL_FPoint getCenter() { return center; }

    SDL_FPoint getVelocity() { return velocity; }

    // get x, get y
    float getX() const { return center.x; }
    float getY() const { return center.y; }

    uint32_t getRadius() const { return radius; }

    // setters
    void setCenter(SDL_FPoint pos) { center = pos; }

    void setVelocity(SDL_FPoint vel) { velocity = vel; }

    void setRadius(int s) { radius = s; }

private:
    SDL_FPoint center{};
    uint32_t radius;

    uint32_t speed;
    SDL_FPoint velocity{};
};

#endif