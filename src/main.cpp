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

  run_sim = true;
  float sim_freq = 1000;          // steps per second
  float sim_step = 1 / sim_freq;  // seconds per step
  std::thread sim_thread([&sim, &run_sim, sim_step]() {
    while (run_sim) {
      sim.update(sim_step);
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<int>(1000 * sim_step)));
    }
  });

  render_engine render(&sim);
  render.register_creation([&sim](float x, float y, float radius) {
    body b1(radius, radius * radius * std::numbers::pi_v<float>, {x, y}, {0, 0},
            body::color::red);

    std::lock_guard sim_guard(sim.get_bodies_mutex());
    sim.add_body(std::move(b1));
  });
  render.run();

  run_sim = false;
  sim_thread.join();
}
