#pragma once

#include <cmath>

namespace simpleworld {

struct vector2d {
  float x;
  float y;

  vector2d operator+(const vector2d other) {
    return {x + other.x, y + other.y};
  };

  vector2d operator-(const vector2d other) {
    return {x - other.x, y - other.y};
  };

  float operator*(vector2d other) { return x * other.x + y * other.y; }

  vector2d operator*(float factor) { return {factor * x, factor * y}; }
  vector2d operator/(float factor) { return {x / factor, y / factor}; }

  float length() { return std::sqrt(x * x + y * y); }
  float length_squared() { return (x * x + y * y); }
};

}  // namespace simpleworld
