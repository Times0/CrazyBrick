
#include <iostream>
#include "../include/utils.h"

int random_int(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

float myclamp(float value, float min, float max) {
    return std::max(min, std::min(value, max));
}


Vector2 getUnitCirclePos(int i, int n, int r) {
    double angle = 2 * M_PI * i / n;
    double x = std::cos(angle) * r + GAME_WIDTH / 2;
    double y = std::sin(angle) * r + GAME_HEIGHT / 2;
    return {x, y};
}

std::vector<Polygon> generateCoords(int n, int h, int min_r, int max_r) {
    std::vector<Polygon> coords;

    for (int r = min_r; r < max_r; r += h) {
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
    return sqrt(dx * dx + dy * dy) + 0.0001;


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


std::unique_ptr<TTF_Font, std::function<void(TTF_Font *)>> loadFont(const std::string &path, int ptSize) {
    TTF_Font *font = TTF_OpenFont(path.c_str(), ptSize);
    if (font == nullptr) {
        std::cerr << "Error: Failed to load font " << TTF_GetError() << std::endl;
        return nullptr;
    }
    return {font, TTF_CloseFont};
}


void drawText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, 100, 100};
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
