#include "../Components/button_grid.hpp"
#include "../Components/histogram.hpp"
#include "../Components/ui_manager.hpp"

#include <SFML/Graphics.hpp>

#include "../neural_net/net.hpp"

using components::Histogram;
using components::ButtonGrid;
using nn::NeuralNet;

inline void handleResize(const std::optional<sf::Event> event, sf::View& view, sf::RenderWindow& window) {
  if(const auto* resized = event->getIf<sf::Event::Resized>()) {
    view.setSize(sf::Vector2f(resized->size));
    view.setCenter(0.5F * sf::Vector2f(resized->size));
    window.setView(view);
  }
}



int main()
{


    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Digit-NN");
    window.setFramerateLimit(144);
    sf::View view = window.getDefaultView();

    // load Neural Net;
    NeuralNet digitNet = nn::NeuralNet::loadFromFile("model97");

    UIManager ui(window, digitNet);

    while (window.isOpen()) {
      while (const std::optional event = window.pollEvent()) {
        ui.handleEvent(*event);
      }
      ui.update();
      ui.draw();
    }
}
