//
// Created by Dorian on 08/05/2024.
//
#include "../include/level_selection.h"


LevelSelection::LevelSelection() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIDTH, GAME_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = loadFont("../assets/fonts/OpenSans-Regular.ttf", 24);
    title_font = loadFont("../assets/fonts/OmegaFlight3D-8M4n0.otf", 90);
    subtitle_font = loadFont("../assets/fonts/OmegaFlight3D-8M4n0.otf", 50);



    // Find number of levels available in the levels directory
    std::string levels_dir = (std::filesystem::current_path() / ".." / "levels").string();

    for (const auto &entry: std::filesystem::directory_iterator(levels_dir)) {
        // only save the basename (no extension)
        // only save txt files
        if (entry.path().extension() != ".txt") {
            continue;
        }
        _levels.push_back(entry.path().filename().replace_extension("").string());
    }
    std::sort(_levels.begin(), _levels.end(), [](const std::string &a, const std::string &b) {
        return a < b;
    });
}

void LevelSelection::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                    playLevel(_selected_level + 1);
                    return;
                case SDLK_DOWN:
                    _selected_level = (_selected_level + 1) % _levels.size();
                    break;
                case SDLK_UP:
                    _selected_level = (_selected_level - 1 + _levels.size()) % _levels.size();
                    break;
                default:
                    break;
            }
        }
    }
}

void LevelSelection::draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Get the title text dimensions
    int title_width, title_height;
    TTF_SizeText(title_font.get(), "Crazy Brick", &title_width, &title_height);

    // Draw title centered
    int title_x = GAME_WIDTH / 2 - title_width / 2;
    int title_y = 100;
    drawText(renderer, title_font.get(), "Crazy Brick", title_x, title_y, SDL_Color{255, 255, 255});


    // Draw subtitle
    int subtitle_width, subtitle_height;
    TTF_SizeText(subtitle_font.get(), "Select a level", &subtitle_width, &subtitle_height);
    int subtitle_x = GAME_WIDTH / 2 - subtitle_width / 2;
    int subtitle_y = title_y + title_height + 50;
    drawText(renderer, subtitle_font.get(), "Select a level", subtitle_x, subtitle_y, SDL_Color{255, 255, 255});


    // Calculate the level rect dimensions
    int level_rect_width = 500;
    int level_rect_height = 70;

    // Draw levels rect around the selected level
    int y = 400;
    for (int i = 0; i < _levels.size(); i++) {
        // Get the level text dimensions
        int level_text_width, level_text_height;
        TTF_SizeText(font.get(), (_levels[i]).c_str(), &level_text_width, &level_text_height);

        int level_rect_x = GAME_WIDTH / 2 - level_rect_width / 2;
        int level_text_x = GAME_WIDTH / 2 - level_text_width / 2;
        SDL_Rect rect = {level_rect_x, y, level_rect_width, level_rect_height};
        SDL_Color color = {255, 255, 255};
        if (i == _selected_level) {
            color = {255, 0, 0};
        }
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderDrawRect(renderer, &rect);
        drawText(renderer, font.get(), _levels[i], level_text_x, y + (level_rect_height - level_text_height) / 2,
                 color);
        y += level_rect_height + 10;
    }

    // Copyrright text
    drawText(renderer, font.get(), "2024 Dorian", 10, GAME_HEIGHT - 30, SDL_Color{255, 255, 255});


    SDL_RenderPresent(renderer);
}

void LevelSelection::playLevel(int level) {
    _level = std::make_unique<Game>(window, renderer);
    _level->loadBricksFromFile(_levels[level - 1]);
    _level->run();
}

void LevelSelection::run() {
    while (_running) {
        SDL_Delay(1000 / 30);
        handleEvents();
        draw();
    }
}
