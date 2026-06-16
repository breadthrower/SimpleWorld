#include "world_simulator.h"

namespace simpleworld {

void world_simulator::update() {
  std::lock_guard guard(bodies_mutex);

  for (body& b : bodies) {
    b.update(_sim_step);
  }

  for (body& b : bodies) {
    b.collideBoundaries(_width, _height);
  }

  for (size_t i = 0; i < bodies.size(); ++i) {
    for (size_t j = i + 1; j < bodies.size(); ++j) {
      bodies[i].collideWith(bodies[j]);
    }
  }
}

void world_simulator::run() {
  _run_sim = true;
  _worker = std::thread([&]() {
    while (_run_sim) {
      update();
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<int>(1000 * _sim_step)));
    }
  });
}

void world_simulator::stop() {
  _run_sim = false;
  _worker.join();
}

float world_simulator::get_total_energy() {
  float energy = 0;
  std::lock_guard guard(bodies_mutex);
  for (body& b : bodies) {
    energy += b.get_mass() * (.5 * b.get_velocity().length_squared() +
                              (sim_height - b.get_position().y) * gravity);
  }
  return energy;
}

}  // namespace simpleworld