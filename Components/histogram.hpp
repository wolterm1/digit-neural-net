#pragma once
#include <SFML/Graphics.hpp>
#include "vector.hpp"

namespace components {


class Histogram : public sf::Drawable {
  public:

  explicit Histogram(sf::Vector2f tSize, size_t count);

  void setPosition(sf::Vector2f target);
  void setFontFromFile(const std::string& filepath);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void updateBars(const lin::Vector<float>& probabilityDistribution);
  void setBarLabels(const std::vector<std::string>& labels);
  void setBarColor(sf::Color target);
  void setLabelColor(sf::Color target);
  void setSize(sf::Vector2f target);
  sf::Vector2f getSize() const;

  private:
  sf::VertexArray barVertices;
  std::vector<sf::Text> barLabels; 

  size_t barCount;
  sf::Vector2f position = {0.0, 0.0};
  sf::Vector2f size;
 // float maxHeight = 100.0F;
 // float barWidth = 10.0F;
 // float spacing = 20.0F;
  sf::Color barColor = sf::Color::Green;
  sf::Font labelFont;

  void initVertices();
  void alignBarLabels();

  float getHeight() const;
  float getBarWidth() const;
  float getSpacing() const;
};

}
