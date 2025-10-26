#pragma once
#include <SFML/Graphics.hpp>

namespace components { 

class Button : public sf::Drawable{
  public:

  sf::Vector2f position;
  sf::RectangleShape rectangle;
  sf::Vector2f size;
  sf::Color color = sf::Color::White;

  Button();
  Button(sf::Vector2f position, sf::Vector2f size);


  void updateStatus(sf::RenderWindow& window, const sf::Event &event);

  private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  bool pressed = false;
  bool hovered = false;
  bool activated = false;
};

}



