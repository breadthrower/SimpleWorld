#include "world_simulator.h"

namespace simpleworld {

void world_simulator::update(float dt) {
  std::lock_guard guard(bodies_mutex);

  for (body& b : bodies) {
    b.update(dt);
  }

  for (body& b : bodies) {
    b.collideBoundaries(width, height);
  }

  for (size_t i = 0; i < bodies.size(); ++i) {
    for (size_t j = i + 1; j < bodies.size(); ++j) {
      bodies[i].collideWith(bodies[j]);
    }
  }
}

}  // namespace simpleworld