#ifndef CRAZYBRICK_GAME_H
#define CRAZYBRICK_GAME_H

#include <SDL2/SDL.h>
#include <vector>


class Game {
public:
    Game(int width, int height);

    ~Game();

    void run();

private:
    void handleEvents(float dt);

    void update(uint dt);

    void render();

    void drawPaddle();


    SDL_Window *window;
    SDL_Renderer *renderer;


    static const uint PADDLE_WIDTH = 100;
    static const uint PADDLE_HEIGHT = 20;

    const uint32_t FPS = 60;
    int paddleX, paddleY;
    uint paddleSpeed = 550;
    bool running = true;

};


#endif //CRAZYBRICK_GAME_H
