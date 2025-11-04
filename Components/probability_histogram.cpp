#include "probability_histogram.hpp"

namespace components {

ProbabilityHistogram::ProbabilityHistogram(size_t count) : barCount(count),
  barVertices(sf::VertexArray(sf::PrimitiveType::Triangles, count * 6)) {
  initVertices();
}

void ProbabilityHistogram::setPosition(sf::Vector2f target) {
  position = target;
  initVertices();
}

void ProbabilityHistogram::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(barVertices);
}

void ProbabilityHistogram::updateBars(const lin::Vector<float>& probabilityDistribution) {
  assert(probabilityDistribution.getSize() == barCount);
  for (size_t i = 0; i < barCount; ++i) {
    size_t idx = i * 6;
    float minHeight = 2.0F;
    float barHeight = std::max(minHeight, maxHeight * probabilityDistribution[i]);
    float targetY = position.y + maxHeight - barHeight;

    barVertices[idx].position.y  = targetY;
    barVertices[idx + 1].position.y = targetY;
    barVertices[idx + 3].position.y = targetY;
  }
}

void ProbabilityHistogram::initVertices() {
  for (size_t i = 0; i < barCount; ++i) {
    size_t idx = i * 6;
    sf::Vector2f barPos = position + sf::Vector2f(i * (spacing+barWidth), 0.0F);

    barVertices[idx].position = barPos + sf::Vector2f(0.0F, defaultHeight);
    barVertices[idx + 1].position = barPos + sf::Vector2f(barWidth, defaultHeight);
    barVertices[idx + 2].position = barPos + sf::Vector2f(barWidth, maxHeight);

    barVertices[idx + 3].position = barPos + sf::Vector2f(0.0F, defaultHeight);
    barVertices[idx + 4].position = barPos + sf::Vector2f(barWidth, maxHeight);
    barVertices[idx + 5].position = barPos + sf::Vector2f(0.0F, maxHeight);

    for(size_t j = 0; j < 6; ++j) {
      barVertices[idx + j].color = sf::Color::Red;
    }
  }
}

}
