
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
    paddle = std::make_unique<Paddle>();
    add_ball(center_x, GAME_HEIGHT - 50);
    game_clock = Clock();
    fps_to_show = 0;
    font = load_font("../assets/fonts/OpenSans-Regular.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
    }

    audio_manager.load_sound((project_root_dir / "assets/sound/bonus1.wav"));
    audio_manager.load_sound((project_root_dir / "assets/sound/brick_collision.wav"));
    audio_manager.load_sound((project_root_dir / "assets/sound/bonus2.wav"));
    audio_manager.load_sound((project_root_dir / "assets/sound/welcome.wav"));
    audio_manager.load_sound((project_root_dir / "assets/sound/win.wav"));
    audio_manager.load_sound((project_root_dir / "assets/sound/start.wav"));
    audio_manager.load_sound((project_root_dir / "assets/sound/paddle_ball.wav"));

    audio_manager.play_sound("start");

    texture_heart = load_texture((project_root_dir / "assets/images/heart.png").string(), renderer);
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
        int type = random_int(0, n - 1);
        bricks.emplace_back(std::make_unique<Brick>(polygon_points, type));
    }

}

void Game::run() {
    while (_running) {
        float dt = game_clock.tick(FPS);

        /* Avoid moving the elements too much in case of lag or freeze */
        if (dt > 0.1) {
            dt = 0.1;
        }
        handle_events(dt);
        update(dt);
        draw();
    }
}

void Game::handle_events(float dt) {
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
    paddle->handleEvents(dt);
}

void Game::update(float dt) {
    for (auto &ball: balls) {
        ball->update(dt);
    }
    powerup_manager.update(dt);

    balls.remove_if([this](const std::unique_ptr<Ball> &ball) {
        if (ball->is_out_of_bounds()) {
            if (balls.size() == 1) {
                lives--;
                if (lives == 0) {
                    _running = false;
                } else {
                    //we add a ball at the paddle position but more in the center
                    add_ball(CENTER_X, GAME_HEIGHT - 50);
                    paddle->reset_pos_speed();
                }
            }
            return true;
        }
        return false;
    });


    // Ball paddle
    for (auto &ball: balls) {
        if (ball->handleSolidCollision(paddle->get_points())) {
            audio_manager.play_sound("ball_collide");
        }
    }

    // Ball Brick
    for (auto &ball: balls) {
        bricks.remove_if([this, &ball](const std::unique_ptr<Brick> &brick) {
            if (ball->handleSolidCollision(brick->get_points())) {
                brick->decrease_collision_count();
                if (brick->get_collision_count() <= 0) {
                    if (random_int(0, 100) < PROBABILITY_POWERUP) {
                        Vector2 pos = brick->get_center();
                        Vector2 vel = ball->velocity;
                        powerup_manager.spawn_random_powerup(pos.x, pos.y, vel.x, vel.y);
                    }
                    audio_manager.play_sound("brick_collision");

                    if (bricks.size() == 1) {
                        _running = false;
                        audio_manager.play_sound("win");
                    }
                    return true;
                }
            }
            return false;
        });
    }

    // Check for collisions with powerups
    powerup_manager.handle_collision_with_paddle(paddle->get_points());
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
        brick->draw(renderer);
    }

    // draw powerups
    powerup_manager.draw(renderer);

    // draw paddle
    paddle->draw(renderer);

    draw_fps();
    if (game_clock.getFrameCount() % (FPS / 4) == 0) {
        fps_to_show = game_clock.get_fps();
    }

    drawLives();

    SDL_RenderPresent(renderer);
}

void Game::draw_fps() {
    drawText(renderer, font.get(), "FPS: " + std::to_string(fps_to_show), 10, 10, {255, 255, 255});
}

void Game::drawLives() {
    //we display the number of lives as a number of hearts
    for (int i = 0; i < lives; i++) {
        SDL_Rect rect = {GAME_WIDTH - 50 - i * 50, 10, 50, 50};
        SDL_RenderCopy(renderer, texture_heart.get(), nullptr, &rect);
    }
}

// Powerups
void Game::add_ball(float x, float y) {
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

void Game::increase_paddle_size() {
    paddle->width += 20;
}

void Game::double_balls() {
    int n = balls.size();
    for (int i = 0; i < n; i++) {
        auto &ball = balls.front();
        add_ball(ball->center.x, ball->center.y);
    }
}

void Game::increase_ball_speed() {
    for (auto &ball: balls) {
        ball->speed *= 2;
    }
}
