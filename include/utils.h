//
// Created by Dorian on 09/05/2024.
//

#pragma once

#include <random>
#include <cmath>
#include "types.h"
#include "config.h"
#include "ball.h"
#include <algorithm>
#include <type_traits>
#include "../include/utils.h"
#include <cmath>
#include <random>
#include <memory>
#include <SDL_ttf.h>
#include <functional>
#include <SDL_image.h>


int random_int(int min, int max);

float myclamp(float value, float min, float max);


Vector2 getUnitCirclePos(int i, int n, int r);

std::vector<Polygon> generateCoords(int n, int h, int min_r, int max_r);

double pointLineDistance(const std::pair<double, double> &point,
                         const std::pair<double, double> &line_start,
                         const std::pair<double, double> &line_end);


bool handlePolygonCircleCollision(const Polygon &polygon_points, Vector2 center, float radius);


std::unique_ptr<TTF_Font, std::function<void(TTF_Font *)>> loadFont(const std::string &path, int ptSize);

void drawText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int x, int y, SDL_Color color);

std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture *)>>
loadTexture(const std::string &file, SDL_Renderer *ren);