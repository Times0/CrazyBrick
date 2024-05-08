#ifndef CRAZYBRICK_GAME_H
#define CRAZYBRICK_GAME_H

#ifdef _WIN32

#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif

#include <vector>
#include "ball.h"
#include <cmath>
#include "config.h"
#include "brick.h"

class game {
public:
    game();

    ~game();

    void run();

private:
    void handleEvents(float dt);

    void update(float dt);

    void draw();

    void drawPaddle();

    SDL_Window *window;
    SDL_Renderer *renderer;

    float center_x = GAME_WIDTH / 2.;
    float center_y = GAME_HEIGHT / 2.;

    float rotation_angle = 0.0f;
    Polygon paddle;
    uint32_t paddleSpeed = PADDLE_SPEED;
    bool running = true;
    bool collision = false;

    // list of balls
    std::vector<ball> balls;
    // list of bricks
    std::vector<brick> bricks;

};

#endif // CRAZYBRICK_GAME_H
