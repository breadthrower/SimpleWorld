#pragma once

#include "body.h"

#include <mutex>
#include <vector>

namespace simpleworld {

class world_simulator {
 public:
  world_simulator(float width, float height) : width(width), height(height) {};
  void add_body(body&& new_body) { bodies.push_back(std::move(new_body)); }
  std::vector<body>& get_bodies() { return bodies; }
  std::mutex& get_bodies_mutex() { return bodies_mutex; }
  void update(float dt);

 private:
  std::mutex bodies_mutex;
  std::vector<body> bodies;
  float width;
  float height;
};

}  // namespace simpleworld