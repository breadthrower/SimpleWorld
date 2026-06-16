#include "body.h"
#include "constants.h"
#include "vector2d.h"

namespace simpleworld {

void body::update(float dt) {
  vel.y += gravity * dt;
  pos.x += vel.x * dt;
  pos.y += vel.y * dt;
}

}  // namespace simpleworld