#include "ball.h"
#include "../include/ball.h"


ball::ball(int x, int y) {
    position = {x, y};
    size = 10;
    speed = 200;
    velocity = {1, 1};
}

void ball::Update(float dt) {
    // Update the position of the ball based on its velocity
    position.x += velocity.x * dt * speed;
    position.y += velocity.y * dt * speed;
}

void ball::Draw(SDL_Renderer *renderer) {
    // Set the color to draw with
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color

    // Draw the ball
    SDL_Rect ballRect = {position.x, position.y, size, size};
    SDL_RenderFillRect(renderer, &ballRect);
}

SDL_Point ball::GetPosition() const {
    return position;
}
