#include "button.hpp"
#include <iostream>

namespace components {


Button::Button() {
  position = {0.0, 0.0};
  size = {0.0, 0.0};
  color = sf::Color::White;
  rectangle = sf::RectangleShape();
}

Button::Button(sf::Vector2f position, sf::Vector2f size) : position(position), size(size) {
  rectangle = sf::RectangleShape();
  rectangle.setSize(size);
  rectangle.setPosition(position);
  rectangle.setFillColor(color);
  rectangle.setOutlineColor(sf::Color::Black); // black outline
  rectangle.setOutlineThickness(2.0F);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates state) const {
  target.draw(rectangle);
}

void Button::updateStatus(sf::RenderWindow& window, const sf::Event& event) {
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
  sf::Vector2f mousePosF(mousePos);


  if (rectangle.getGlobalBounds().contains(mousePosF)) {
    rectangle.setFillColor(sf::Color(100,100,100,255));
    hovered = true;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!activated) {
        color = sf::Color::Black;
        rectangle.setFillColor(color);
        activated = true;
      } else {
        color = sf::Color::White;
        rectangle.setFillColor(color);
        activated = false;
      }
    }
  } else {
    rectangle.setFillColor(color);
    hovered = false;
  }

}

}

