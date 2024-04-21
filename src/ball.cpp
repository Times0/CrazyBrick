#include "../include/ball.h"
#include "../include/config.h"

ball::ball(float x, float y, float vx, float vy)
{
    position = {x, y};
    velocity = {vx, vy};
    speed = BALL_SPEED;
    size = BALL_SIZE;
}

void ball::update(float dt)
{
    position = {position.x + velocity.x * static_cast<float>(speed) * dt, position.y + velocity.y * static_cast<float>(speed) * dt};
}

void ball::Draw(SDL_Renderer *renderer) const
{
    // Draw white ball
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect ballRect = {static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(size), static_cast<int>(size)};
    SDL_RenderFillRect(renderer, &ballRect);
}
