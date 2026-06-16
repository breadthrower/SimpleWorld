#pragma once

#include "constants.h"
#include "vector2d.h"

#include <cmath>
#include <optional>

namespace simpleworld {

class body {

 public:
  enum class color { red, green, blue };

  body(float radius, float mass, const vector2d& pos, const vector2d& vel,
       color color = color::red)
      : radius(radius), mass(mass), pos(pos), vel(vel), color(color) {}
  body(body&) = delete;
  body& operator=(body&) = delete;
  body(body&&) = default;
  body& operator=(body&&) = default;

  void set_position(const vector2d& new_pos) { pos = new_pos; }
  vector2d get_position() const { return pos; }
  void set_velocity(const vector2d& new_vel) { vel = new_vel; }
  vector2d get_velocity() const { return vel; }
  float get_radius() const { return radius; }
  float get_mass() const { return mass; }
  color get_color() const { return color; }

  void update(float dt);

  void collideBoundaries(float width, float height) {
    if (pos.x < radius) {
      pos.x = radius;
      vel.x = -vel.x * boundary_elasticity;
    } else if (pos.x > width - radius) {
      pos.x = width - radius;
      vel.x = -vel.x * boundary_elasticity;
    }

    if (pos.y < radius) {
      pos.y = radius;
      vel.y = -vel.y * boundary_elasticity;
    } else if (pos.y > height - radius) {
      pos.y = height - radius;
      vel.y = -vel.y * boundary_elasticity;
    }
  }

  void collideWith(body& other) {
    vector2d delta = other.pos - pos;
    float dist = delta.length();
    vector2d n = delta / dist;
    vector2d k = vel - other.vel;
    if (dist < radius + other.radius) {
      float overlap = -0.5 * (dist - radius - other.radius);
      pos = pos - (delta / dist) * overlap * 1.1;
      other.pos = other.pos + (delta / dist) * overlap * 1.1;

      float p = 2.0 * (n.x * k.x + n.y * k.y) / (mass + other.mass);
      vel = vel - n * other.mass * p;
      other.vel = other.vel + n * mass * p;
    }
  }

 private:
  float radius;
  float mass;
  vector2d pos;
  vector2d vel;
  color color;
};

}  // namespace simpleworld
