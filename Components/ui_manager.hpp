#pragma once
#include <SFML/Graphics.hpp>
#include "button_grid.hpp"
#include "histogram.hpp"
#include "../neural_net/net.hpp"

class UIManager {
public:
    UIManager(sf::RenderWindow& window, nn::NeuralNet& net);

    void updateLayout();

    void draw();

    void handleEvent(const sf::Event& event);

    void update();

private:

    sf::RenderWindow& window;
    sf::View view;
    nn::NeuralNet net;

    components::ButtonGrid grid;
    components::Histogram histogram;


    const sf::Color bg = sf::Color(20, 25, 30);
    const sf::Color barColor = sf::Color(255, 230, 100);
    const sf::Color labelColor = sf::Color(200, 200, 200);

    void handleResize(const sf::Event& event);
};


