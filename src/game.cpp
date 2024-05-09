
#include "../include/game.h"
#include "../include/config.h"
#include "../include/types.h"
#include "../include/brick.h"
#include "../include/clock.h"

#include <cmath>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <random>

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


Vector2 getUnitCirclePos(int i, int n, int r) {
    double angle = 2 * M_PI * i / n;
    double x = std::cos(angle) * r + CENTER_X;
    double y = std::sin(angle) * r + CENTER_Y;
    return {x, y};
}

std::vector<Polygon> generateCoords(int n, int h) {
    std::vector<Polygon> coords;

    for (int r = 25; r < 300; r += h) {
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

bool handleBallPolygonCollision(const Polygon &polygon_points, ball &ball) {
    // This function is dependent on the order of the points in the polygon

    for (size_t i = 0; i < polygon_points.size(); ++i) {
        size_t next_index = (i + 1) % polygon_points.size();
        const Vector2 &p1 = polygon_points[i];
        const Vector2 &p2 = polygon_points[next_index];

        float distance = pointLineDistance({ball.getX(), ball.getY()}, {p1.x, p1.y}, {p2.x, p2.y});
        if (distance < ball.getRadius()) {
            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            float normal_x = -dy;
            float normal_y = dx;

            float length = std::sqrt(normal_x * normal_x + normal_y * normal_y);
            normal_x /= length;
            normal_y /= length;

            // Set the ball's position just outside
            ball.setCenter({ball.getX() + normal_x * (ball.getRadius() - distance),
                            ball.getY() + normal_y * (ball.getRadius() - distance)});

            float dot_product = ball.getVelocity().x * normal_x + ball.getVelocity().y * normal_y;

            ball.setVelocity({ball.getVelocity().x - 2 * dot_product * normal_x,
                              ball.getVelocity().y - 2 * dot_product * normal_y});
            return true;
        }
    }
    return false;
}


game::game() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH,
                              GAME_HEIGHT,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // create the paddle
    paddle = {{GAME_WIDTH / 2, GAME_HEIGHT / 2 - CIRCLE_RADIUS},
              {GAME_WIDTH / 2, GAME_HEIGHT / 2 + CIRCLE_RADIUS},
              {GAME_WIDTH / 2, GAME_HEIGHT / 2 + CIRCLE_RADIUS + PADDLE_HEIGHT},
              {GAME_WIDTH / 2, GAME_HEIGHT / 2 - CIRCLE_RADIUS + PADDLE_HEIGHT}};

    // add the starting ball
    balls.emplace_back(GAME_WIDTH / 2, GAME_HEIGHT - 2, 0.5f, -1.0f);
    balls.emplace_back(GAME_WIDTH / 2, GAME_HEIGHT - 2, -0.5f, -1.0f);
    balls.emplace_back(GAME_WIDTH / 2, GAME_HEIGHT - 2, 0.0f, -1.0f);
    balls.emplace_back(GAME_WIDTH / 2, GAME_HEIGHT - 2, 0.1f, -1.0f);

    // create the bricks
    std::vector<std::vector<Vector2>> coords = generateCoords(20, 50);
    bricks.reserve(coords.size());
    for (auto &points: coords) {
        Polygon polygon_points;
        for (auto &point: points) {
            polygon_points.emplace_back(point.x, point.y);
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 4); // Range: [0, 4]
        int type = dis(gen);
        bricks.emplace_back(polygon_points, type);
    }

    // Powerup manager



    // clock
    gameClock = Clock();
    fps_to_show = 0;


    // load font
    font = TTF_OpenFont("../OpenSans-Regular.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
    }

}

game::~game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void game::run() {
    while (running) {
        float dt = gameClock.tick(FPS);

        /* Avoid moving the elements too much in case of lag / window moving */
        if (dt > 0.1) {
            dt = 0.1;
        }
        std::cout << dt << std::endl;
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
        rotation_angle += 0.1f * dt * static_cast<float>(paddleSpeed);
    }
    if (keyboardStates[SDL_SCANCODE_RIGHT]) {
        rotation_angle -= 0.1f * dt * static_cast<float>(paddleSpeed);
    }

    if (keyboardStates[SDL_SCANCODE_LEFT] xor keyboardStates[SDL_SCANCODE_RIGHT]) {
        rotation_angle = std::fmod(rotation_angle, 2 * M_PI);

        const float offset_x = PADDLE_WIDTH / 2 * cos(M_PI / 2 - rotation_angle);
        const float offset_y = PADDLE_WIDTH / 2 * sin(M_PI / 2 - rotation_angle);

        paddle[3].x = std::cos(rotation_angle) * CIRCLE_RADIUS + center_x - offset_x;
        paddle[3].y = std::sin(rotation_angle) * CIRCLE_RADIUS + center_y + offset_y;

        paddle[2].x = std::cos(rotation_angle) * CIRCLE_RADIUS + center_x + offset_x;
        paddle[2].y = std::sin(rotation_angle) * CIRCLE_RADIUS + center_y - offset_y;

        paddle[1].x = std::cos(rotation_angle) * (CIRCLE_RADIUS + PADDLE_HEIGHT) + center_x + offset_x;
        paddle[1].y = std::sin(rotation_angle) * (CIRCLE_RADIUS + PADDLE_HEIGHT) + center_y - offset_y;

        paddle[0].x = std::cos(rotation_angle) * (CIRCLE_RADIUS + PADDLE_HEIGHT) + center_x - offset_x;
        paddle[0].y = std::sin(rotation_angle) * (CIRCLE_RADIUS + PADDLE_HEIGHT) + center_y + offset_y;


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
    for (auto &ball: balls) {
        handleBallPolygonCollision(paddle, ball);
    }

    // Check for collisions with bricks
    for (auto &ball: balls) {
        // remove bricks that are hit. Randomely spawn powerups
        bricks.erase(std::remove_if(bricks.begin(), bricks.end(), [&ball, this](brick &brick) {
            if (handleBallPolygonCollision(brick.getPoints(), ball)) {
                float x, y, vx, vy;
                x = brick.getCenter().x;
                y = brick.getCenter().y;
                vx = 0.0f;
                vy = 1.0f;
                powerup_manager.spawnPowerup(x, y, vx, vy);
                return true;
            }
            return false;
        }), bricks.end());
    }

    // update powerups
    powerup_manager.update(dt);

}

void game::draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawPaddle();
    for (auto &ball: balls) {
        ball.draw(renderer);
    }


    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    for (int i = 0; i < 360; i++) {
        SDL_RenderDrawPoint(renderer, static_cast<int>(cos(i * M_PI / 180) * CIRCLE_RADIUS + center_x),
                            static_cast<int>(sin(i * M_PI / 180) * CIRCLE_RADIUS + center_y));
    }

    // draw bricks using their coords
    for (auto &brick: bricks) {
        brick.draw(renderer);
    }

    // update powerups
    powerup_manager.draw(renderer);

    // draw the number of fps bottom update the number every 120 frames
    drawFPS();
    if (gameClock.getFrameCount() % (FPS / 4) == 0) {
        fps_to_show = gameClock.get_fps();
    }


    SDL_RenderPresent(renderer);
}

void game::drawPaddle() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (size_t i = 0; i < paddle.size(); i++) {
        SDL_RenderDrawLineF(renderer,
                            paddle[i].x,
                            paddle[i].y,
                            paddle[(i + 1) % paddle.size()].x,
                            paddle[(i + 1) % paddle.size()].y);
    }
}

void game::drawFPS() {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, std::to_string(fps_to_show).c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {0, 0, 100, 100};
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}