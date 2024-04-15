#include "Game.h"

Game::Game(int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    paddleX = (width / 2) - (PADDLE_WIDTH / 2);
    paddleY = height - PADDLE_HEIGHT - 10;

}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::run() {
    const uint32_t FRAME_TIME = 1000 / FPS; // Time for one frame in milliseconds
    uint32_t lastFrameTime = 0;

    while (running) {
        Uint32 frameNow = SDL_GetTicks();
        Uint32 timeSinceLastFrame = frameNow - lastFrameTime;
        lastFrameTime = frameNow;
        float dt = (float) timeSinceLastFrame / 1000.0f;
        if (FRAME_TIME > timeSinceLastFrame) {
            SDL_Delay(FRAME_TIME - timeSinceLastFrame);
        }
        handleEvents(dt);
        render();
    }
}

void Game::handleEvents(float dt) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_MOUSEMOTION) {
            paddleX = event.motion.x - (PADDLE_WIDTH / 2);
        }
    }
    const Uint8 *keyboardStates = SDL_GetKeyboardState(nullptr);
    if (keyboardStates[SDL_SCANCODE_LEFT]) {
        paddleX -= (int) ((float) paddleSpeed * dt);
    }
    if (keyboardStates[SDL_SCANCODE_RIGHT]) {
        paddleX += (int) ((float) paddleSpeed * dt);
    }
}

void Game::update(uint dt) {

}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawPaddle();

    SDL_RenderPresent(renderer);
}

void Game::drawPaddle() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect paddleRect = {paddleX, paddleY, PADDLE_WIDTH, PADDLE_HEIGHT};
    SDL_RenderFillRect(renderer, &paddleRect);
}

