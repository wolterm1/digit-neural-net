#include <SFML/Graphics.hpp>
#include "vector.hpp"

namespace components {

class ProbabilityHistogram : public sf::Drawable {
  public:

  explicit ProbabilityHistogram(size_t count);
  void initVertices();

  void setPosition(sf::Vector2f target);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void updateBars(const lin::Vector<float>& probabilityDistribution);

  private:
  sf::VertexArray barVertices;
  size_t barCount;
  sf::Vector2f position = {0.0, 0.0};
  float maxHeight = 100.0F;
  float barWidth = 10.0F;
  float spacing = 20.0F;
  float defaultHeight = 0.5F * maxHeight;

};

}
