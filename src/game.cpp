#include "../include/game.h"
#include "../include/config.h"
#include <math.h>

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
        // else if (event.type == SDL_MOUSEMOTION)
        // {
        //     paddle.x = event.motion.x - (paddle.w / 2);
        // }
    }
    const Uint8 *keyboardStates = SDL_GetKeyboardState(nullptr);

    if (keyboardStates[SDL_SCANCODE_LEFT])
    {
        rotation_angle += 0.1f * dt * paddleSpeed;
    }
    if (keyboardStates[SDL_SCANCODE_RIGHT])
    {
        rotation_angle -= 0.1f * dt * paddleSpeed;
    }
    if (keyboardStates[SDL_SCANCODE_LEFT] || keyboardStates[SDL_SCANCODE_RIGHT])
    {
        rotation_angle = fmod(rotation_angle, 2 * M_PI);
        paddle.x = cos(rotation_angle) * CIRCLE_RADIUS + rotation_center.x - PADDLE_WIDTH / 2;
        paddle.y = sin(rotation_angle) * CIRCLE_RADIUS + rotation_center.y - PADDLE_HEIGHT / 2;
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

            // Calculate the new velocity
            float relativeIntersectX = paddle.x + (paddle.w / 2) - ball.GetPosition().x; // distance from the center of the paddle
            float normalizedRelativeIntersectionX = relativeIntersectX / (paddle.w / 2); // normalize to [-1, 1]
            float bounceAngle = normalizedRelativeIntersectionX * (float)M_PI / 3;       // bounce angle in radians
            ball.SetVelocity({-sin(bounceAngle), -cos(bounceAngle)});
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

    // draw circle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < 360; i++)
    {
        SDL_RenderDrawPoint(renderer, static_cast<int>(cos(i * M_PI / 180) * CIRCLE_RADIUS + rotation_center.x),
                            static_cast<int>(sin(i * M_PI / 180) * CIRCLE_RADIUS + rotation_center.y));
    }
    

    SDL_RenderPresent(renderer);
}

void game::drawPaddle()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle);
}
