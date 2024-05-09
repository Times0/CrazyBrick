#include <cmath>
#include <SDL.h>
#include <iostream>
#include "config.h"
#include "types.h"
#include "utils.h"

constexpr float PI = 3.14159265358979323846f;

class Paddle {
public:
    Paddle() {
        points = {
                {CENTER_X - PADDLE_WIDTH / 2, CENTER_Y + CIRCLE_RADIUS},
                {CENTER_X + PADDLE_WIDTH / 2, CENTER_Y + CIRCLE_RADIUS},
                {CENTER_X + PADDLE_WIDTH / 2, CENTER_Y + CIRCLE_RADIUS + PADDLE_HEIGHT},
                {CENTER_X - PADDLE_WIDTH / 2, CENTER_Y + CIRCLE_RADIUS + PADDLE_HEIGHT}
        };
    }

    void update(bool isRotatingLeft, bool isRotatingRight, float dt) {
        rotation_angle = std::fmod(rotation_angle, 2 * PI);


        const int offset_x = width / 2 * std::cos(PI / 2 - rotation_angle);
        const int offset_y = width / 2 * std::sin(PI / 2 - rotation_angle);

        points[3].x = std::cos(rotation_angle) * CIRCLE_RADIUS + CENTER_X - offset_x;
        points[3].y = std::sin(rotation_angle) * CIRCLE_RADIUS + CENTER_Y + offset_y;

        points[2].x = std::cos(rotation_angle) * CIRCLE_RADIUS + CENTER_X + offset_x;
        points[2].y = std::sin(rotation_angle) * CIRCLE_RADIUS + CENTER_Y - offset_y;

        points[1].x = std::cos(rotation_angle) * (CIRCLE_RADIUS + height) + CENTER_X + offset_x;
        points[1].y = std::sin(rotation_angle) * (CIRCLE_RADIUS + height) + CENTER_Y - offset_y;

        points[0].x = std::cos(rotation_angle) * (CIRCLE_RADIUS + height) + CENTER_X - offset_x;
        points[0].y = std::sin(rotation_angle) * (CIRCLE_RADIUS + height) + CENTER_Y + offset_y;

        if (speed < 0 && isRotatingLeft || speed > 0 && isRotatingRight || !isRotatingLeft && !isRotatingRight) {
            speed = 0;
        }

        if (isRotatingLeft) {
            speed += PADDLE_ACCELERATION;
        } else if (isRotatingRight) {
            speed -= PADDLE_ACCELERATION;
        }

        speed = myclamp(speed, (float) -max_speed, (float) max_speed);
        rotation_angle += speed * dt * 0.1;
    }

    const Polygon &getPoints() {
        return points;
    }

    void handleEvents(float dt) {
        const Uint8 *keyboardStates = SDL_GetKeyboardState(nullptr);
        bool isRotatingLeft = keyboardStates[SDL_SCANCODE_LEFT];
        bool isRotatingRight = keyboardStates[SDL_SCANCODE_RIGHT];
        update(isRotatingLeft, isRotatingRight, dt);
    }

    void draw(SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (size_t i = 0; i < points.size(); i++) {
            SDL_RenderDrawLineF(renderer,
                                points[i].x,
                                points[i].y,
                                points[(i + 1) % points.size()].x,
                                points[(i + 1) % points.size()].y);
        }
    }

    int width = PADDLE_WIDTH;
private:
    int height = PADDLE_HEIGHT;
    float speed = 0;
    int max_speed = PADDLE_MAX_SPEED;

    float rotation_angle = PI / 2;
    Polygon points;
};