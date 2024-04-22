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

    void Draw(SDL_Renderer *renderer) const;

    // getters
    SDL_FPoint GetPosition() { return position; }

    SDL_FPoint GetVelocity() { return velocity; }

    uint32_t GetSize() const { return size; }

    // setters
    void SetPosition(SDL_FPoint pos) { position = pos; }

    void SetVelocity(SDL_FPoint vel) { velocity = vel; }

    void SetSize(int s) { size = s; }

private:
    SDL_FPoint position{};
    SDL_FPoint velocity{};
    uint32_t speed;
    uint32_t size;
};

#endif