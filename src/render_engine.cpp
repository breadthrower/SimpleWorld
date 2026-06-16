#include "render_engine.h"

namespace simpleworld {

render_engine::render_engine(world_simulator* sim)
    : _sim(sim),
      _window(sf::VideoMode({static_cast<unsigned int>(sim->get_width()),
                             static_cast<unsigned int>(sim->get_height())}),
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
          auto dist =
              static_cast<sf::Vector2f>(_mouseMovedPos - _mousePressedPos);
          float radius = dist.length();
          _creationCallback(_mousePressedPos.x, _mousePressedPos.y, radius);
        }
      }
    }

    if (_leftButtonPressed) {
      auto dist = static_cast<sf::Vector2f>(_mouseMovedPos - _mousePressedPos);
      float radius = dist.length();

      sf::CircleShape shape(radius);
      shape.setPosition({static_cast<sf::Vector2f>(_mousePressedPos)});
      shape.setFillColor(sf::Color::Magenta);
      _window.draw(shape);
    }

    {
      std::lock_guard sim_guard(_sim->get_bodies_mutex());
      auto& bodies = _sim->get_bodies();
      for (auto& body : bodies) {
        sf::CircleShape shape(body.get_radius());
        switch (body.get_color()) {
          case body::color::red:
            shape.setFillColor(sf::Color::Red);
            break;
          case body::color::green:
            shape.setFillColor(sf::Color::Green);
            break;
          case body::color::blue:
            shape.setFillColor(sf::Color::Blue);
            break;
        }
        shape.setPosition({body.get_position().x - body.get_radius(),
                           body.get_position().y - body.get_radius()});

        _window.draw(shape);
      }
    }
    _window.display();
  }
}

}  // namespace simpleworld