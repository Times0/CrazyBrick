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

std::vector<Polygon> generateCoords(int n, int h);

double pointLineDistance(const std::pair<double, double> &point, const std::pair<double, double> &line_start,
                         const std::pair<double, double> &line_end);

bool handlePolygonCircleCollision(const Polygon &polygon_points, Vector2 center, float radius);