#include "../include/ball.h"

Ball::Ball(double x, double y, double vx, double vy) : center(x, y), velocity(vx, vy) {
    speed = BALL_SPEED;
    radius = BALL_RADIUS;
}

void Ball::update(float dt) {
    center = {center.x + velocity.x * static_cast<float>(speed) * dt,
              center.y + velocity.y * static_cast<float>(speed) * dt};
}

void Ball::draw(SDL_Renderer *renderer) const {
    // draw white Ball
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180;
        SDL_RenderDrawPoint(renderer, static_cast<int>(center.x + radius * cos(angle)),
                            static_cast<int>(center.y + radius * sin(angle)));
    }
}

bool Ball::handleSolidCollision(const Polygon &polygon_points) {
    // This function is dependent on the order of the points in the polygon
    float min_ditance = 1000000;
    std::pair<Vector2, Vector2> mypair = {{0, 0},
                                          {0, 0}};
    for (size_t i = 0; i < polygon_points.size(); ++i) {
        size_t next_index = (i + 1) % polygon_points.size();
        const Vector2 &p1 = polygon_points[i];
        const Vector2 &p2 = polygon_points[next_index];

        float distance = pointLineDistance({center.x, center.y}, {p1.x, p1.y}, {p2.x, p2.y});

        if (distance < min_ditance) {
            min_ditance = distance;
            mypair = {p1, p2};
        }
    }
    if (min_ditance < radius) {
        bounce(mypair, min_ditance);
        // play sound
        return true;

    }
    return false;
}

void Ball::bounce(std::pair<Vector2, Vector2> mypair, float min_ditance) {
    const Vector2 &p1 = mypair.first;
    const Vector2 &p2 = mypair.second;

    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float normal_x = -dy;
    float normal_y = dx;

    float length = std::sqrt(normal_x * normal_x + normal_y * normal_y);
    normal_x /= length;
    normal_y /= length;

    // Set the Ball's position just outside
    center = {center.x + normal_x * (radius - min_ditance),
              center.y + normal_y * (radius - min_ditance)};

    float dot_product = velocity.x * normal_x + velocity.y * normal_y;

    velocity = {velocity.x - 2 * dot_product * normal_x,
                velocity.y - 2 * dot_product * normal_y};
}

bool Ball::isOutOfBounds() const {
    return center.y - radius > GAME_HEIGHT || center.y + radius < 0 || center.x - radius > GAME_WIDTH ||
           center.x + radius < 0;
}

