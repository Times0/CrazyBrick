#ifndef CRAZYBRICK_GAME_H
#define CRAZYBRICK_GAME_H

#ifdef _WIN32

#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif

#include <vector>
#include "ball.h"
#include <math.h>
#include "config.h"

typedef struct
{
    SDL_FPoint topleft;
    SDL_FPoint topright;
    SDL_FPoint bottomleft;
    SDL_FPoint bottomright;
} Paddle;


class game
{
public:
    game();

    ~game();

    void run();

private:
    void handleEvents(float dt);

    void update(float dt);

    void render();

    void drawPaddle();

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Point rotation_center = {GAME_WIDTH / 2, GAME_HEIGHT / 2};
    float rotation_angle = 0.0f;
    Paddle paddle = {
        {GAME_WIDTH / 2 - PADDLE_WIDTH / 2, GAME_HEIGHT - PADDLE_HEIGHT},
        {GAME_WIDTH / 2 + PADDLE_WIDTH / 2, GAME_HEIGHT - PADDLE_HEIGHT},
        {GAME_WIDTH / 2 - PADDLE_WIDTH / 2, GAME_HEIGHT},
        {GAME_WIDTH / 2 + PADDLE_WIDTH / 2, GAME_HEIGHT}
    };

    uint32_t paddleSpeed = PADDLE_SPEED;
    bool running = true;

    // list of balls
    std::vector<ball> balls;
};

#endif // CRAZYBRICK_GAME_H
