// types.hpp
#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>
#include <utility>

struct Vector2 {
    double x, y;
    Vector2(double x, double y) : x(x), y(y) {}
};

using Polygon = std::vector<Vector2>;


#endif // TYPES_HPP