#pragma once

#include <SFML/Graphics.hpp>

#include <functional>

namespace simpleworld {

class render_engine {
 public:
  render_engine(float width, float height);
  void run();

  void register_creation(
      std::function<void(float, float, float)> creationCallback) {
    _creationCallback = creationCallback;
  }

 private:
  sf::RenderWindow _window;
  std::vector<std::unique_ptr<sf::Shape>> _shapes;
  sf::Vector2i _mousePressedPos;
  sf::Vector2i _mouseMovedPos;
  bool _leftButtonPressed = false;

  std::function<void(float, float, float)> _creationCallback;
}
}  // namespace simpleworld
