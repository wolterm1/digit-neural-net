#include <iostream>
#include "../Components/button.hpp"
#include "../Components/button_grid.hpp"
#include <SFML/Graphics.hpp>

#include "../neural_net/net.hpp"
#include "helper.hpp"

using components::Button;
using components::ButtonGrid;

inline void handleResize(const std::optional<sf::Event> event, sf::View& view, sf::RenderWindow& window) {
  if(const auto* resized = event->getIf<sf::Event::Resized>()) {
    view.setSize(sf::Vector2f(resized->size));
    view.setCenter(0.5F * sf::Vector2f(resized->size));
    window.setView(view);
  }
}

void classificationHandler(const sf::Event& event, ButtonGrid& bGrid, nn::NeuralNet& net) {
  if (const auto& keyRelease = event.getIf<sf::Event::KeyReleased>()) {
    if (keyRelease->code == sf::Keyboard::Key::C) {
      std::cout << nn::getIndexOfMax(net.classify(bGrid.getNormalizedInput())) << std::flush;
    }
  }
}

int main()
{
    auto window= sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);
    sf::View view = window.getDefaultView();

    ButtonGrid bGrid(28, 28);
    bGrid.setCellSize(25.0);
    bGrid.setPosition(sf::Vector2f(25.0, 25.0));


    // load Neural Net;
    auto net = nn::NeuralNet::load_from_file("test");

    while (window.isOpen()) {
      while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        handleResize(event, view, window);
        bGrid.updateStatus(window, *event);
        classificationHandler(*event, bGrid, net);
      }

      window.clear(sf::Color(100, 100, 100, 10));
      window.draw(bGrid.triaVertices);
      window.display();
    }
}
