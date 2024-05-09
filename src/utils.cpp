//
// Created by Dorian on 09/05/2024.
//

#include "../include/utils.h"
#include <cmath>
#include <random>


int myRandomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}


Vector2 getUnitCirclePos(int i, int n, int r) {
    double angle = 2 * M_PI * i / n;
    double x = std::cos(angle) * r + GAME_WIDTH / 2;
    double y = std::sin(angle) * r + GAME_HEIGHT / 2;
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

double pointLineDistance(const std::pair<double, double> &point,
                         const std::pair<double, double> &line_start,
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


bool handlePolygonCircleCollision(const Polygon &polygon_points, Vector2 center, float radius) {
    // This function is dependent on the order of the points in the polygon

    for (size_t i = 0; i < polygon_points.size(); ++i) {
        size_t next_index = (i + 1) % polygon_points.size();
        const Vector2 &p1 = polygon_points[i];
        const Vector2 &p2 = polygon_points[next_index];

        float distance = pointLineDistance({center.x, center.y}, {p1.x, p1.y}, {p2.x, p2.y});
        if (distance < radius) {
            return true;
        }
    }
    return false;
}
