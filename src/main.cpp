#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <numbers>
#include <thread>
#include <vector>

#include "world_simulator.h"

using namespace simpleworld;

int main() {

  bool run_sim = false;

  world_simulator sim(sim_width, sim_height);

  sf::RenderWindow window(
      sf::VideoMode({static_cast<unsigned int>(sim_width),
                     static_cast<unsigned int>(sim_height)}),
      "SimpleWorld");

  std::vector<std::unique_ptr<sf::Shape>> shapes;

  run_sim = true;
  float sim_freq = 1000;
  float sim_step = 1 / sim_freq;
  std::thread sim_thread([&sim, &run_sim, sim_step]() {
    while (run_sim) {
      sim.update(sim_step);
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<int>(1000 * sim_step)));
    }
  });

  bool leftButtonPressed = false;
  vector2d mousePressedPos;
  vector2d mouseMovedPos;

  while (window.isOpen()) {

    window.clear();

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();

      if (const auto* mouseButtonPressed =
              event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
          mousePressedPos = {
              static_cast<float>(mouseButtonPressed->position.x),
              static_cast<float>(mouseButtonPressed->position.y)};
          leftButtonPressed = true;
          std::cout << "left button pressed" << std::endl;
        }
      }

      if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {

        mouseMovedPos = {static_cast<float>(mouseMoved->position.x),
                         static_cast<float>(mouseMoved->position.y)};
      }

      if (const auto* mouseButtonReleased =
              event->getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
          std::cout << "left button released" << std::endl;
          leftButtonPressed = false;
          vector2d dist = mouseMovedPos - mousePressedPos;
          float radius = dist.length();
          body b1(radius, radius * radius * std::numbers::pi_v<float>,
                  {mouseButtonReleased->position.x,
                   mouseButtonReleased->position.y},
                  {10, 0}, body::color::red);

          std::lock_guard sim_guard(sim.get_bodies_mutex());
          sim.add_body(std::move(b1));
        }
      }
    }

    if (leftButtonPressed) {
      vector2d dist = mouseMovedPos - mousePressedPos;
      float radius = dist.length();
      std::cout << "radius" << radius << std::endl;

      auto shape = std::make_unique<sf::CircleShape>(radius);
      shape->setPosition({mousePressedPos.x, mousePressedPos.y});
      shape->setFillColor(sf::Color::Magenta);
      window.draw(*shape);
    }

    {
      std::lock_guard sim_guard(sim.get_bodies_mutex());
      shapes.clear();
      auto& bodies = sim.get_bodies();
      for (auto& body : bodies) {
        auto shape = std::make_unique<sf::CircleShape>(body.get_radius());
        switch (body.get_color()) {
          case body::color::red:
            shape->setFillColor(sf::Color::Red);
            break;
          case body::color::green:
            shape->setFillColor(sf::Color::Green);
            break;
          case body::color::blue:
            shape->setFillColor(sf::Color::Blue);
            break;
        }
        shapes.push_back(std::move(shape));
      }

      for (size_t i = 0; i < bodies.size(); ++i) {
        shapes[i]->setPosition(
            {bodies[i].get_position().x - bodies[i].get_radius(),
             bodies[i].get_position().y - bodies[i].get_radius()});
      }
    }

    for (const auto& shape : shapes) {
      window.draw(*shape);
    }
    window.display();
  }

  run_sim = false;
  sim_thread.join();
}
