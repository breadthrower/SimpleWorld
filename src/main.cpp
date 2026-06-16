#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <numbers>
#include <thread>
#include <vector>

#include "render_engine.h"
#include "world_simulator.h"

using namespace simpleworld;

int main() {

  bool run_sim = false;

  world_simulator sim(sim_width, sim_height);

  render_engine render(&sim);
  render.register_creation([&sim](float x, float y, float radius) {
    body b1(radius, radius * radius * std::numbers::pi_v<float>, {x, y}, {0, 0},
            body::color::red);

    std::lock_guard sim_guard(sim.get_bodies_mutex());
    sim.add_body(std::move(b1));
  });

  sim.run();
  render.run();

  sim.stop();
}
