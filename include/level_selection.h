//
// Created by Dorian on 08/05/2024.
//

#ifndef BREAKOUT_LEVEL_SELECTION_H
#define BREAKOUT_LEVEL_SELECTION_H

#include "game.h"


class LevelSelection {
public:
    LevelSelection();

    ~LevelSelection() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }

    void run();


    void draw();

    void handleEvents();

    void playLevel(int level);

private:
    std::unique_ptr<Game> _level;
    bool _running = true;
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::unique_ptr<TTF_Font, std::function<void(TTF_Font *)>> font;
    std::unique_ptr<TTF_Font, std::function<void(TTF_Font *)>> title_font;
    std::unique_ptr<TTF_Font, std::function<void(TTF_Font *)>> subtitle_font;

    std::vector<std::string> _levels;
    int _selected_level = 0;
};

#endif //BREAKOUT_LEVEL_SELECTION_H
