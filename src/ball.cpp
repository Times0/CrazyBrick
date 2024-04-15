#include "../include/ball.h"

ball::ball(int x, int y) {
    position = {x, y};
    posx = (float) x;
    posy = (float) y;
    size = 10;
    speed = 200;
    velocity = {1, 1};
}

void ball::Update(float dt) {
    // Update the position of the ball based on its velocity
    posx += (float) velocity.x * dt * (float) speed;
    posy += (float) velocity.y * dt * (float) speed;
    position = {(int) posx, (int) posy};
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
