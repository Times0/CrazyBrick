#include "game.h"


game::game(int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    paddle.x = width / 2 - paddle.w / 2;
    paddle.y = height - paddle.h - 10;

    // add one ball
    balls.push_back(ball(300, 300));

}

game::~game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void game::run() {
    const uint32_t FRAME_TIME = 1000 / FPS;
    uint32_t lastFrameTime = 0;

    while (running) {
        // Slow down the loop to the desired frame rate
        Uint32 frameNow = SDL_GetTicks();
        Uint32 timeSinceLastFrame = frameNow - lastFrameTime;
        lastFrameTime = frameNow;
        float dt = (float) timeSinceLastFrame / 1000.0f;
        if (FRAME_TIME > timeSinceLastFrame) {
            SDL_Delay(FRAME_TIME - timeSinceLastFrame);
        }
        handleEvents(dt);
        update(dt);
        render();
    }
}

void game::handleEvents(float dt) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_MOUSEMOTION) {
            paddle.x = event.motion.x - (paddle.w / 2);
        }
    }
    const Uint8 *keyboardStates = SDL_GetKeyboardState(nullptr);
    if (keyboardStates[SDL_SCANCODE_LEFT]) {
        paddle.x -= (int) ((float) paddleSpeed * dt);
    }
    if (keyboardStates[SDL_SCANCODE_RIGHT]) {
        paddle.x += (int) ((float) paddleSpeed * dt);
    }
}

void game::update(uint dt) {
    for (auto &ball:balls){
        ball.Update(dt);
    }
}

void game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawPaddle();
    for (auto &ball: balls) {
        ball.Draw(renderer);
    }

    SDL_RenderPresent(renderer);
}

void game::drawPaddle() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle);
}

