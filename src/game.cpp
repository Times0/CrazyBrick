#include "../include/game.h"
#include "../include/config.h"

game::game()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH, GAME_HEIGHT,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    paddle.x = GAME_WIDTH / 2 - paddle.w / 2;
    paddle.y = GAME_HEIGHT - paddle.h - 10;

    // add the starting ball
    balls.emplace_back(GAME_WIDTH / 2, GAME_HEIGHT - 2, 0.5f, -1.0f);
}

game::~game()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void game::run()
{
    const uint32_t FRAME_TIME = 1000 / FPS;
    uint32_t lastFrameTime = 0;

    while (running)
    {
        // Slow down the loop to the desired frame rate
        Uint32 frameNow = SDL_GetTicks();
        Uint32 timeSinceLastFrame = frameNow - lastFrameTime;
        lastFrameTime = frameNow;
        float dt = (float)timeSinceLastFrame / 1000.0f;
        if (timeSinceLastFrame < FRAME_TIME)
        {
            SDL_Delay(FRAME_TIME - timeSinceLastFrame);
        }
        handleEvents(dt);
        update(dt);
        render();
    }
}

void game::handleEvents(float dt)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            paddle.x = event.motion.x - (paddle.w / 2);
        }
    }
    const Uint8 *keyboardStates = SDL_GetKeyboardState(nullptr);
    if (keyboardStates[SDL_SCANCODE_LEFT])
    {
        paddle.x -= (int)((float)paddleSpeed * dt);
    }
    if (keyboardStates[SDL_SCANCODE_RIGHT])
    {
        paddle.x += (int)((float)paddleSpeed * dt);
    }
}

void game::update(float dt)
{
    for (auto &ball : balls)
    {
        ball.update(dt);
    }

    // Check for collisions with borders
    for (auto &ball : balls)
    {
        if (ball.GetPosition().x < 0)
        {
            ball.SetPosition({0, ball.GetPosition().y});
            ball.SetVelocity({-ball.GetVelocity().x, ball.GetVelocity().y});
        }
        if (ball.GetPosition().x + ball.GetSize() > GAME_WIDTH)
        {
            ball.SetPosition({static_cast<float>(GAME_WIDTH) - ball.GetSize(), ball.GetPosition().y});
            ball.SetVelocity({-ball.GetVelocity().x, ball.GetVelocity().y});
        }
        if (ball.GetPosition().y < 0)
        {
            ball.SetPosition({ball.GetPosition().x, 0});
            ball.SetVelocity({ball.GetVelocity().x, -ball.GetVelocity().y});
        }
    }

    // Check for collisions with paddle
    for (auto &ball : balls)
    {
        if (ball.GetPosition().y + ball.GetSize() > paddle.y && ball.GetPosition().y < paddle.y + paddle.h &&
            ball.GetPosition().x + ball.GetSize() > paddle.x && ball.GetPosition().x < paddle.x + paddle.w)
        {
            ball.SetPosition({ball.GetPosition().x, static_cast<float>(paddle.y - ball.GetSize())});
            ball.SetVelocity({ball.GetVelocity().x, -ball.GetVelocity().y});
        }
    }
}

void game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawPaddle();
    for (auto &ball : balls)
    {
        ball.Draw(renderer);
    }

    SDL_RenderPresent(renderer);
}

void game::drawPaddle()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle);
}
