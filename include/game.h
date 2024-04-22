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

    SDL_Point rotation_center = {GAME_WIDTH / 2, GAME_HEIGHT / 2};
    float rotation_angle = 0.0f;
    SDL_Rect paddle = {static_cast<int>(cos(rotation_angle) * CIRCLE_RADIUS + rotation_center.x - PADDLE_WIDTH / 2),
                       static_cast<int>(sin(rotation_angle) * CIRCLE_RADIUS + rotation_center.y - PADDLE_HEIGHT / 2),
                       PADDLE_WIDTH, PADDLE_HEIGHT};
                       
    uint32_t paddleSpeed = PADDLE_SPEED;
    bool running = true;

    // list of balls
    std::vector<ball> balls;

};


#endif //CRAZYBRICK_GAME_H
