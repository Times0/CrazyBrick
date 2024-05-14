
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
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>


Game::Game(SDL_Window *window, SDL_Renderer *renderer) : window(window), renderer(renderer) {
    project_root_dir = std::filesystem::current_path() / "..";
    powerup_manager.bind(this);
    paddle = Paddle();
    addBall(center_x, GAME_HEIGHT - 50);
    gameClock = Clock();
    fps_to_show = 0;
    font = loadFont("../assets/fonts/OpenSans-Regular.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
    }

    audio_manager.LoadSound((project_root_dir / "assets/sound/welcome.wav").string(), "welcome");
    audio_manager.LoadSound((project_root_dir / "assets/sound/ball_collide.wav").string(), "ball_collide");

    audio_manager.PlaySound("welcome");
}

Game::~Game() = default;


void Game::loadBricksFromFile(const std::string &name) {

    auto full_path = project_root_dir / "levels" / (name + ".txt");
    std::cout << "Loading file: " << full_path << std::endl;

    std::ifstream
            file(full_path);


    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << full_path << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Read the first line
    std::istringstream iss(line);

    int n, brick_height, min_circle_radius, max_circle_radius;
    if (!(iss >> n >> brick_height >> min_circle_radius >> max_circle_radius)) {
        std::cerr << "Error: Invalid file format" << std::endl;
        file.close();
        return;
    }

    // Check if the values are valid
    if (n <= 0 || brick_height <= 0 || min_circle_radius < 0 || max_circle_radius <= 0) {
        std::cerr << "Error: Invalid values in file" << std::endl;
        file.close();
        return;
    }
    file.close();

    // create the bricks
    std::vector<std::vector<Vector2>> coords = generateCoords(n, brick_height, min_circle_radius, max_circle_radius);
    for (auto &points: coords) {
        Polygon polygon_points;
        for (auto &point: points) {
            polygon_points.emplace_back(point.x, point.y);
        }
        int type = myRandomInt(0, n - 1);
        bricks.emplace_back(polygon_points, type);
    }

}

void Game::run() {
    while (_running) {
        float dt = gameClock.tick(FPS);

        /* Avoid moving the elements too much in case of lag or freeze */
        if (dt > 0.1) {
            dt = 0.1;
        }
        handleEvents(dt);
        update(dt);
        draw();
    }
}

void Game::handleEvents(float dt) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                _running = false;
            }
        }
    }
    paddle.handleEvents(dt);
}

void Game::update(float dt) {
    for (auto &ball: balls) {
        ball->update(dt);
    }
    powerup_manager.update(dt);

    balls.remove_if([this](const std::unique_ptr<Ball> &ball) {
        if (ball->isOutOfBounds()) {
            if (balls.size() == 1) {
                _running = false;
            }
            return true;
        }
        return false;
    });


    // Ball paddle
    for (auto &ball: balls) {
        if (ball->handleSolidCollision(paddle.getPoints())) {
            audio_manager.PlaySound("ball_collide");
        }
    }

    // Ball Brick
    for (auto &ball: balls) {
        // remove bricks that are hit. Randomly spawn powerups
        bricks.remove_if([&ball, this](brick &brick) {
            if (ball->handleSolidCollision(brick.getPoints())) {
                if (myRandomInt(0, 100) < PROBABILITY_POWERUP) {
                    double x, y, vx, vy;
                    x = brick.getCenter().x;
                    y = brick.getCenter().y;
                    vx = ball->velocity.x;
                    vy = ball->velocity.y;
                    powerup_manager.spawnPowerup(x, y, vx, vy);
                }
                return true;
            }
            return false;
        });
    }

    // Check for collisions with powerups
    powerup_manager.handlePaddleCollision(paddle.getPoints());
}

void Game::draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (auto &ball: balls) {
        ball->draw(renderer);
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

    // draw powerups
    powerup_manager.draw(renderer);

    // draw paddle
    paddle.draw(renderer);

    drawFPS();
    if (gameClock.getFrameCount() % (FPS / 4) == 0) {
        fps_to_show = gameClock.get_fps();
    }

    SDL_RenderPresent(renderer);
}

void Game::drawFPS() {
    drawText(renderer, font.get(), "FPS: " + std::to_string(fps_to_show), 10, 10, {255, 255, 255});
}

// Powerups
void Game::addBall(float x, float y) {
    const float speed = 0.7f;
    float vx, vy;

    if (x == 0 && y == 0) {
        // spawn at the first Ball's position
        x = balls.front().get()->center.x;
        y = balls.front().get()->center.y;
    }

    // Point to the center
    vx = center_x - x;
    vy = center_y - y;


    // Normalize the velocity vector
    const float magnitude = std::sqrt(vx * vx + vy * vy);
    vx = vx / magnitude * speed;
    vy = vy / magnitude * speed;

    balls.push_back(std::make_unique<Ball>(x, y, vx, vy));
}

void Game::increasePaddleSize() {
    paddle.width += 2;
}