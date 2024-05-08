
#include "../include/game.h"
#include "../include/config.h"
#include "../include/types.h"
#include "../include/brick.h"

#include <vector>
#include <cmath>
#include <iostream>

class Vector2f;

double pointLineDistance(const std::pair<double, double> &point, const std::pair<double, double> &line_start,
                         const std::pair<double, double> &line_end) {
    double x = point.first;
    double y = point.second;
    double x1 = line_start.first;
    double y1 = line_start.second;
    double x2 = line_end.first;
    double y2 = line_end.second;

    double A = x - x1;
    double B = y - y1;
    double C = x2 - x1;
    double D = y2 - y1;

    double dot = A * C + B * D;
    double len_sq = C * C + D * D;
    double param = -1;
    if (len_sq != 0) {
        param = dot / len_sq;
    }

    double xx, yy;
    if (param < 0) {
        xx = x1;
        yy = y1;
    } else if (param > 1) {
        xx = x2;
        yy = y2;
    } else {
        xx = x1 + param * C;
        yy = y1 + param * D;
    }

    double dx = x - xx;
    double dy = y - yy;
    return sqrt(dx * dx + dy * dy);


}


const int CENTER_X = GAME_WIDTH / 2;
const int CENTER_Y = GAME_HEIGHT / 2;

struct Vector2 {
    int x, y;

    Vector2(int x, int y) : x(x), y(y) {}
};

Vector2 getUnitCirclePos(int i, int n, int r) {
    double angle = 2 * M_PI * i / n;
    int x = static_cast<int>(std::cos(angle) * r + CENTER_X);
    int y = static_cast<int>(std::sin(angle) * r + CENTER_Y);
    return {x, y};
}

std::vector<std::vector<Vector2>> generateCoords(int n, int h) {
    std::vector<std::vector<Vector2>> coords;

    for (int r = 50; r < 300; r += h) {
        for (int i = 0; i < n; ++i) {
            Vector2 p1 = getUnitCirclePos(i, n, r);
            Vector2 p2 = getUnitCirclePos((i + 1) % n, n, r);
            Vector2 p3 = getUnitCirclePos((i + 1) % n, n, r + h);
            Vector2 p4 = getUnitCirclePos(i, n, r + h);

            coords.push_back({p1, p2, p3, p4});
        }
    }

    return coords;
}

game::game() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH,
                              GAME_HEIGHT,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // add the starting ball
    balls.emplace_back(GAME_WIDTH / 2, GAME_HEIGHT - 2, 0.5f, -1.0f);

    // create the bricks
    std::vector<std::vector<Vector2>> coords = generateCoords(50, 25);
    bricks.reserve(coords.size());
    for (auto &points: coords) {
        Polygon polygon_points;
        for (auto &point: points) {
            polygon_points.emplace_back(point.x, point.y);
        }
        bricks.emplace_back(polygon_points);
    }

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
        if (timeSinceLastFrame < FRAME_TIME) {
            SDL_Delay(FRAME_TIME - timeSinceLastFrame);
        }
        handleEvents(dt);
        update(dt);
        draw();
    }
}

void game::handleEvents(float dt) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_MOUSEMOTION) {
            // move the ball to mouse position
//            int x, y;
//            SDL_GetMouseState(&x, &y);
//            balls[0].setCenter({static_cast<float>(x), static_cast<float>(y)});
        }
    }
    const Uint8 *keyboardStates = SDL_GetKeyboardState(nullptr);

    if (keyboardStates[SDL_SCANCODE_LEFT]) {
        rotation_angle += 0.1f * dt * paddleSpeed;
    }
    if (keyboardStates[SDL_SCANCODE_RIGHT]) {
        rotation_angle -= 0.1f * dt * paddleSpeed;
    }

    if (keyboardStates[SDL_SCANCODE_LEFT] xor keyboardStates[SDL_SCANCODE_RIGHT]) {
        rotation_angle = fmod(rotation_angle, 2 * M_PI);

        const float offset_x = PADDLE_WIDTH / 2 * cos(M_PI / 2 - rotation_angle);
        const float offset_y = PADDLE_WIDTH / 2 * sin(M_PI / 2 - rotation_angle);

        paddle.topleft.x = cos(rotation_angle) * CIRCLE_RADIUS + rotation_center.x - offset_x;
        paddle.topleft.y = sin(rotation_angle) * CIRCLE_RADIUS + rotation_center.y + offset_y;

        paddle.topright.x = cos(rotation_angle) * CIRCLE_RADIUS + rotation_center.x + offset_x;
        paddle.topright.y = sin(rotation_angle) * CIRCLE_RADIUS + rotation_center.y - offset_y;

        paddle.bottomleft.x = cos(rotation_angle) * (CIRCLE_RADIUS + PADDLE_HEIGHT) + rotation_center.x - offset_x;
        paddle.bottomleft.y = sin(rotation_angle) * (CIRCLE_RADIUS + PADDLE_HEIGHT) + rotation_center.y + offset_y;

        paddle.bottomright.x = cos(rotation_angle) * (CIRCLE_RADIUS + PADDLE_HEIGHT) + rotation_center.x + offset_x;
        paddle.bottomright.y = sin(rotation_angle) * (CIRCLE_RADIUS + PADDLE_HEIGHT) + rotation_center.y - offset_y;
    }
}

void game::update(float dt) {
    for (auto &ball: balls) {
        ball.update(dt);
    }

    // Check for collisions with borders
    for (auto &ball: balls) {
        if (ball.getCenter().x - ball.getRadius() < 0) {
            ball.setCenter({static_cast<float>(ball.getRadius()), ball.getCenter().y});
            ball.setVelocity({-ball.getVelocity().x, ball.getVelocity().y});
        }
        if (ball.getCenter().x + ball.getRadius() > GAME_WIDTH) {
            ball.setCenter({static_cast<float>(GAME_WIDTH - ball.getRadius()), ball.getCenter().y});
            ball.setVelocity({-ball.getVelocity().x, ball.getVelocity().y});
        }
        if (ball.getCenter().y - ball.getRadius() < 0) {
            ball.setCenter({ball.getCenter().x, static_cast<float>(ball.getRadius())});
            ball.setVelocity({ball.getVelocity().x, -ball.getVelocity().y});
        }
        if (ball.getCenter().y + ball.getRadius() > GAME_HEIGHT) {
            ball.setCenter({ball.getCenter().x, static_cast<float>(GAME_HEIGHT - ball.getRadius())});
            ball.setVelocity({ball.getVelocity().x, -ball.getVelocity().y});
        }
    }

    // Check for collisions with paddle

    const std::vector<std::pair<float, float>> polygon_points = {
            {paddle.topleft.x,     paddle.topleft.y},
            {paddle.topright.x,    paddle.topright.y},
            {paddle.bottomright.x, paddle.bottomright.y},
            {paddle.bottomleft.x,  paddle.bottomleft.y}
    };

    for (auto &ball: balls) {
        float min_dist = std::numeric_limits<float>::infinity();
        SDL_Point closest_point_start, closest_point_end;


        // Iterate over the polygon points
        for (size_t i = 0; i < polygon_points.size(); ++i) {
            size_t next_index = (i + 1) % polygon_points.size();
            const std::pair<float, float> &p1 = polygon_points[i];
            const std::pair<float, float> &p2 = polygon_points[next_index];

            float distance = pointLineDistance({ball.getX(), ball.getY()}, p1, p2);
            if (distance < min_dist) {
                min_dist = distance;
                closest_point_start = {static_cast<int>(p1.first), static_cast<int>(p1.second)};
                closest_point_end = {static_cast<int>(p2.first), static_cast<int>(p2.second)};
            }
        }

        collision = false;
        if (min_dist <= ball.getRadius()) {
            collision = true;
            float dx = closest_point_end.x - closest_point_start.x;
            float dy = closest_point_end.y - closest_point_start.y;
            float normal_x = -dy;
            float normal_y = dx;

            float length = sqrt(normal_x * normal_x + normal_y * normal_y);
            normal_x /= length;
            normal_y /= length;

            // Set the ball's position just outside the paddle
            ball.setCenter({ball.getX() - normal_x * (ball.getRadius() - min_dist),
                            ball.getY() - normal_y * (ball.getRadius() - min_dist)});


            float dot_product = ball.getVelocity().x * normal_x + ball.getVelocity().y * normal_y;

            ball.setVelocity({ball.getVelocity().x - 2 * dot_product * normal_x,
                              ball.getVelocity().y - 2 * dot_product * normal_y});
        }
    }
}

void game::draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawPaddle();
    for (auto &ball: balls) {
        ball.draw(renderer);
    }

    // draw circle
    if (collision) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }
    for (int i = 0; i < 360; i++) {
        SDL_RenderDrawPoint(renderer, static_cast<int>(cos(i * M_PI / 180) * CIRCLE_RADIUS + rotation_center.x),
                            static_cast<int>(sin(i * M_PI / 180) * CIRCLE_RADIUS + rotation_center.y));
    }

    // draw bricks using their coords
    for (auto &brick: bricks) {
        brick.draw(renderer);
    }


    SDL_RenderPresent(renderer);
}

void game::drawPaddle() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Point points[] = {
            {static_cast<int>(paddle.topleft.x),     static_cast<int>(paddle.topleft.y)},
            {static_cast<int>(paddle.topright.x),    static_cast<int>(paddle.topright.y)},
            {static_cast<int>(paddle.bottomright.x), static_cast<int>(paddle.bottomright.y)},
            {static_cast<int>(paddle.bottomleft.x),  static_cast<int>(paddle.bottomleft.y)},
            {static_cast<int>(paddle.topleft.x),     static_cast<int>(paddle.topleft.y)}};

    SDL_RenderDrawLines(renderer, points, 5);
}

