#ifndef CRAZYBRICK_BALL_H
#define CRAZYBRICK_BALL_H

#include <string>
#include <SDL2/SDL.h>

class ball {
public:
    ball(int x, int y);

    void Update(float dt);

    void Draw(SDL_Renderer *renderer);

    SDL_Point GetPosition() const;

private:
    SDL_Point position;
    SDL_Point velocity;
    uint speed;
    uint size;
};

#endif