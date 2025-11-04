#include <iostream>
#include "../Components/button_grid.hpp"
#include "../Components/probability_histogram.hpp"
#include <SFML/Graphics.hpp>

#include "../neural_net/net.hpp"
#include "helper.hpp"

using components::ProbabilityHistogram;
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
    auto window= sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);
    sf::View view = window.getDefaultView();

    ButtonGrid bGrid(28, 28);
    bGrid.setCellSize(25.0);
    bGrid.setPosition(sf::Vector2f(25.0, 25.0));

    ProbabilityHistogram probHist(10);
    probHist.setPosition(sf::Vector2f(500.0, 500.0));

    // load Neural Net;
    NeuralNet digitNet = nn::NeuralNet::loadFromFile("test");

    while (window.isOpen()) {
      while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        handleResize(event, view, window);

        bGrid.updateStatus(window, *event);

        const lin::Vector<float>& normBrightness = nn::normalizeImage(bGrid.getImage());
        auto dist = digitNet.classify(normBrightness);
        probHist.updateBars(dist);
      }
      window.clear(sf::Color(100, 100, 100, 10));
      window.draw(bGrid);
      window.draw(probHist);
      window.display();
    }
}
