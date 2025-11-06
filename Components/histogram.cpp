#include "histogram.hpp"

namespace components {

Histogram::Histogram(sf::Vector2f tSize, size_t count) : size(tSize), barCount(count),
  barVertices(sf::VertexArray(sf::PrimitiveType::Triangles, count * 6))
  {
  initVertices();
}

void Histogram::setPosition(sf::Vector2f target) {
  position = target;
  initVertices();
  alignBarLabels();
}

void Histogram::setSize(sf::Vector2f target) {
  size = target;
  initVertices();
  alignBarLabels();
}

sf::Vector2f Histogram::getSize() const {
  return size;
}

void Histogram::setFontFromFile(const std::string& filepath) {
  if(!labelFont.openFromFile(filepath)) {
    throw std::runtime_error("Failed to open font " + filepath);
  }
}

void Histogram::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(barVertices);
  for (const auto& label : barLabels) {
    target.draw(label);
  }
}

void Histogram::updateBars(const lin::Vector<float>& probabilityDistribution) {
  assert(probabilityDistribution.getSize() == barCount);
  float maxHeight = getHeight();
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

void Histogram::setBarLabels(const std::vector<std::string>& labels) {
  for (size_t i = 0; i < labels.size(); ++i) {
    barLabels.push_back(sf::Text(labelFont, labels[i]));
  }
  alignBarLabels();

}

void Histogram::initVertices() {
  float maxHeight = getHeight();
  float spacing = getSpacing();
  float barWidth = getBarWidth();
  for (size_t i = 0; i < barCount; ++i) {
    size_t idx = i * 6;
    sf::Vector2f barPos = position + sf::Vector2f(i * (spacing + barWidth), 0.0F);

    barVertices[idx].position = barPos + sf::Vector2f(0.0F, 0.0F);
    barVertices[idx + 1].position = barPos + sf::Vector2f(barWidth, 0.0F);
    barVertices[idx + 2].position = barPos + sf::Vector2f(barWidth, maxHeight);

    barVertices[idx + 3].position = barPos + sf::Vector2f(0.0F, 0.0F);
    barVertices[idx + 4].position = barPos + sf::Vector2f(barWidth, maxHeight);
    barVertices[idx + 5].position = barPos + sf::Vector2f(0.0F, maxHeight);

    for(size_t j = 0; j < 6; ++j) {
      barVertices[idx + j].color = barColor;
    }
  }
}

void Histogram::alignBarLabels() {

  float barWidth = getBarWidth();
  // set size
  for (size_t i = 0; i < barLabels.size(); ++i) {
    barLabels[i].setCharacterSize(static_cast<unsigned int>(barWidth));
  }
  // set position
  for (size_t i = 0; i < barLabels.size(); ++i) {
    auto labelBound = barLabels[i].getLocalBounds();
    float barPosX = (i * (getSpacing() + barWidth));
    float x = barPosX + (0.5F * (barWidth - labelBound.size.x)) - labelBound.position.x;
    float y = getHeight();
    barLabels[i].setPosition(position + sf::Vector2f(x, y));
  }
}

void Histogram::setBarColor(sf::Color target) {
  barColor = target;
}

void Histogram::setLabelColor(sf::Color target) {
  for (auto& el : barLabels) {
    el.setFillColor(target);
  }
}

float Histogram::getHeight() const {
  return size.y;
}

float Histogram::getBarWidth() const {
  return 0.7F * size.x / static_cast<float>(barCount);
}

float Histogram::getSpacing() const {
  return 0.3F * size.x / static_cast<float>(barCount);
}



}
