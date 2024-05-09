#include "../include/ball.h"
#include "../include/config.h"
#include "../include/utils.h"

ball::ball(double x, double y, double vx, double vy) : center(x, y), velocity(vx, vy) {
    speed = BALL_SPEED;
    radius = BALL_SIZE;
}

void ball::update(float dt) {
    center = {center.x + velocity.x * static_cast<float>(speed) * dt,
              center.y + velocity.y * static_cast<float>(speed) * dt};
}

void ball::draw(SDL_Renderer *renderer) const {
    // draw white ball
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180;
        SDL_RenderDrawPoint(renderer, static_cast<int>(center.x + radius * cos(angle)),
                            static_cast<int>(center.y + radius * sin(angle)));
    }
}

bool ball::handleSolidCollision(const Polygon &polygon_points) {
    // This function is dependent on the order of the points in the polygon
    for (size_t i = 0; i < polygon_points.size(); ++i) {
        size_t next_index = (i + 1) % polygon_points.size();
        const Vector2 &p1 = polygon_points[i];
        const Vector2 &p2 = polygon_points[next_index];

        float distance = pointLineDistance({center.x, center.y}, {p1.x, p1.y}, {p2.x, p2.y});
        if (distance < radius) {
            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            float normal_x = -dy;
            float normal_y = dx;

            float length = std::sqrt(normal_x * normal_x + normal_y * normal_y);
            normal_x /= length;
            normal_y /= length;

            // Set the ball's position just outside
            center = {center.x + normal_x * (radius - distance),
                      center.y + normal_y * (radius - distance)};

            float dot_product = velocity.x * normal_x + velocity.y * normal_y;

            velocity = {velocity.x - 2 * dot_product * normal_x,
                        velocity.y - 2 * dot_product * normal_y};
            return true;
        }
    }
    return false;

}

