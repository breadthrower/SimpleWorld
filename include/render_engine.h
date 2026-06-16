#pragma once

#include "world_simulator.h"

#include <SFML/Graphics.hpp>

#include <functional>

namespace simpleworld {

class render_engine {
 public:
  render_engine(world_simulator* sim);
  void run();

  void register_creation(
      std::function<void(float, float, float)> creationCallback) {
    _creationCallback = creationCallback;
  }

 private:
  sf::RenderWindow _window;
  sf::Font _font;
  sf::Text _totalEnergyText;
  world_simulator* _sim;
  sf::Vector2i _mousePressedPos;
  sf::Vector2i _mouseMovedPos;
  bool _leftButtonPressed = false;

  std::function<void(float, float, float)> _creationCallback;
};
}  // namespace simpleworld
