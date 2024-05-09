
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


Game::Game() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH,
                              GAME_HEIGHT,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


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

    // powerup manager
    powerup_manager.bind(this);

    // paddle
    paddle = Paddle();

    // clock
    gameClock = Clock();
    fps_to_show = 0;

    // load font
    font = TTF_OpenFont("../OpenSans-Regular.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
    }
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::run() {
    while (running) {
        float dt = gameClock.tick(FPS);

        /* Avoid moving the elements too much in case of lag / window moving */
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
            running = false;
        }
    }
    paddle.handleEvents(dt);
}

void Game::update(float dt) {
    for (auto &ball: balls) {
        ball.update(dt);
    }

    // Check for collisions with borders
    for (auto &ball: balls) {
        if (ball.center.x - ball.radius < 0) {
            ball.center = {static_cast<float>(ball.radius), ball.center.y};
            ball.velocity.x = -ball.velocity.x;
        }
        if (ball.center.x + ball.radius > GAME_WIDTH) {
            ball.center = {static_cast<float>(GAME_WIDTH - ball.radius), ball.center.y};
            ball.velocity.x = -ball.velocity.x;
        }
        if (ball.center.y - ball.radius < 0) {
            ball.center = {ball.center.x, static_cast<float>(ball.radius)};
            ball.velocity.y = -ball.velocity.y;
        }
        if (ball.center.y + ball.radius > GAME_HEIGHT) {
            ball.center = {ball.center.x, static_cast<float>(GAME_HEIGHT - ball.radius)};
            ball.velocity.y = -ball.velocity.y;
        }
    }

    // Check for collisions with paddle
    for (auto &ball: balls) {
        ball.handleSolidCollision(paddle.getPoints());
    }

    // Check for collisions with bricks
    for (auto &ball: balls) {
        // remove bricks that are hit. Randomly spawn powerups
        bricks.erase(std::remove_if(bricks.begin(), bricks.end(), [&ball, this](brick &brick) {
            if (ball.handleSolidCollision(brick.getPoints())) {
                float x, y;
                x = brick.getCenter().x;
                y = brick.getCenter().y;
                powerup_manager.spawnPowerup(x, y, 0, 1.0f);
                return true;
            }
            return false;
        }), bricks.end());
    }

    // update powerups
    powerup_manager.update(dt);

    // Check for collisions with powerups
    powerup_manager.handlePaddleCollision(paddle.getPoints());

}

void Game::draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

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
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, std::to_string(fps_to_show).c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {0, 0, 100, 100};
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Powerups
void Game::addBall() {
    balls.emplace_back(GAME_WIDTH / 2, GAME_HEIGHT - 2, 0.5f, -1.0f);
}

void Game::increasePaddleSize() {
    // increase the size of the paddle

}