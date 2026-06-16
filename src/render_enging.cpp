#include "render_enginge.h"

namespace simpleworld {

render_engine::render_engine(float width, float height)
    : _window(sf::VideoMode({static_cast<unsigned int>(width),
                             static_cast<unsigned int>(height)}),
              "SimpleWorld") {}

void render_engine::run() {
  while (_window.isOpen()) {

    _window.clear();

    while (const std::optional event = _window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        _window.close();

      if (const auto* mouseButtonPressed =
              event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
          _mousePressedPos = mouseButtonPressed->position;
          _leftButtonPressed = true;
        }
      }

      if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {

        _mouseMovedPos = mouseMoved->position;
      }

      if (const auto* mouseButtonReleased =
              event->getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
          _leftButtonPressed = false;
          auto dist = _mouseMovedPos - _mousePressedPos;
          float radius = dist.length();
          _creationCallback(_mousePressedPos.x, _mousePressedPos.y, radius);
        }
      }
    }

    if (_leftButtonPressed) {
      auto dist = _mouseMovedPos - _mousePressedPos;
      float radius = dist.length();

      auto shape = std::make_unique<sf::CircleShape>(radius);
      shape->setPosition({_mousePressedPos.x, _mousePressedPos.y});
      shape->setFillColor(sf::Color::Magenta);
      _window.draw(*shape);
    }
  }
}

}  // namespace simpleworld