//
// Created by Dorian on 09/05/2024.
//

#pragma once

#include <random>
#include <cmath>
#include "types.h"
#include "config.h"
#include "ball.h"


int myRandomInt(int min, int max);

Vector2 getUnitCirclePos(int i, int n, int r);

std::vector<Polygon> generateCoords(int n, int h, int min_r, int max_r);

double pointLineDistance(const std::pair<double, double> &point, const std::pair<double, double> &line_start,
                         const std::pair<double, double> &line_end);

bool handlePolygonCircleCollision(const Polygon &polygon_points, Vector2 center, float radius);

#include <algorithm>
#include <type_traits>

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr T myclamp(T value, T min_val, T max_val) {
    return std::max(min_val, std::min(value, max_val));
}