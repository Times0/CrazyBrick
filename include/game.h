#ifndef CRAZYBRICK_GAME_H
#define CRAZYBRICK_GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "ball.h"


class game {
public:
    game(int width, int height);

    ~game();

    void run();

private:
    void handleEvents(float dt);

    void update(float dt);

    void render();

    void drawPaddle();


    SDL_Window *window;
    SDL_Renderer *renderer;

    const uint32_t FPS = 144;
    SDL_Rect paddle = {0, 0, 100, 20};
    uint paddleSpeed = 500;
    bool running = true;

    // list of balls
    std::vector<ball> balls;

};


#endif //CRAZYBRICK_GAME_H
