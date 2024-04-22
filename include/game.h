#ifndef CRAZYBRICK_GAME_H
#define CRAZYBRICK_GAME_H

#ifdef _WIN32

#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif

#include <vector>
#include "ball.h"
#include "config.h"


class game {
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

    SDL_Rect paddle = {0, 0, PADDLE_WIDTH, PADDLE_HEIGHT};
    uint32_t paddleSpeed = PADDLE_SPEED;
    bool running = true;

    // list of balls
    std::vector<ball> balls;

};


#endif //CRAZYBRICK_GAME_H
