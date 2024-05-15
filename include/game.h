#ifndef CRAZYBRICK_GAME_H
#define CRAZYBRICK_GAME_H

#ifdef _WIN32

#include <SDL.h>
#include <SDL_ttf.h>

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <vector>
#include <cmath>
#include <filesystem>
#include "config.h"
#include "brick.h"
#include "clock.h"
#include "utils.h"
#include "ball.h"
#include "powerup.h"
#include "paddle.h"
#include "audiomanager.h"

class PowerupManager;

class Game {
public:
    Game(SDL_Window *window, SDL_Renderer *renderer);

    ~Game();

    void run();

    void add_ball(float x = 0.f, float y = 0.f);

    void increase_paddle_size();

    void double_balls();

    void increase_ball_speed();

protected:
    void handle_events(float dt);

    friend class LevelSelection;

    void update(float dt);

    void draw();

    SDL_Window *window;
    SDL_Renderer *renderer;

    float center_x = GAME_WIDTH / 2.;
    float center_y = GAME_HEIGHT / 2.;

    bool _running = true;

    // paddle
    std::unique_ptr<Paddle> paddle;

    // balls
    std::list<std::unique_ptr<Ball>> balls;

    // bricks
    std::list<std::unique_ptr<Brick>> bricks;

    Clock game_clock;
    int fps_to_show = 0;
    int lives = 3;

    // font
    std::unique_ptr<TTF_Font, std::function<void(TTF_Font *)>> font;
    PowerupManager powerup_manager;
    AudioManager audio_manager;

    // project root dir
    std::filesystem::path project_root_dir;

    void draw_fps();

    void loadBricksFromFile(const std::string &filename);

    void spawn_powerup(Vector2 pos, Vector2 vel);

    void drawLives();

    std::shared_ptr<SDL_Texture> texture_heart;


};

#endif // CRAZYBRICK_GAME_H
