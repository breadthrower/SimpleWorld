#pragma once

#include "body.h"

#include <mutex>
#include <thread>
#include <vector>

namespace simpleworld {

class world_simulator {
 public:
  world_simulator(float width, float height)
      : _width(width), _height(height), _sim_step(1 / sim_freq) {};
  float get_width() const { return _width; }
  float get_height() const { return _height; }
  void add_body(body&& new_body) { bodies.push_back(std::move(new_body)); }
  std::vector<body>& get_bodies() { return bodies; }
  std::mutex& get_bodies_mutex() { return bodies_mutex; }
  void run();
  void stop();

  float get_total_energy();

 private:
  void update();
  bool _run_sim = false;
  float _sim_step;

  std::thread _worker;
  std::mutex bodies_mutex;
  std::vector<body> bodies;
  float _width;
  float _height;
};

}  // namespace simpleworld