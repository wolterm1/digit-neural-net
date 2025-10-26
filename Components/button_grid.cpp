// button_grid.cpp
#include "button_grid.hpp"

namespace components {

ButtonGrid::ButtonGrid(size_t rows, size_t columns) : rows(rows), columns(columns) {
  // grid contains rectangles made out of 2 Triangles with 3 vertices each;
  triaVertices = sf::VertexArray(sf::PrimitiveType::Triangles, rows * columns * 6);
  quadColored = lin::Matrix<bool>(rows, columns, false);
  initVertices();
}


void ButtonGrid::setCellSize(float size) {
  cellSize = size;
  initVertices();
}

void ButtonGrid::updateCell(size_t row, size_t col, sf::Color color) {
  std::cout << "Updating cell : " << row << ", " << col << '\n';
  size_t idx = (row * columns + col) * 6;
    for (size_t i = 0; i < 6; ++i) {
      triaVertices[idx + i].color = color;
    }
}

void ButtonGrid::updateStatus(sf::RenderWindow& window, const sf::Event& event) {
  if(const auto& mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
    if(mousePress->button == sf::Mouse::Button::Left) {
      colorQuadBelowCursor(window, sf::Color::Black);
    } else if (mousePress->button == sf::Mouse::Button::Right) {
      eraseColorBelowCursor(window);
    }
  }
  else if (const auto& mouseMove = event.getIf<sf::Event::MouseMoved>()) {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      colorQuadBelowCursor(window, sf::Color::Black);
    } else if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
      eraseColorBelowCursor(window);
    }
  }
  else if (const auto& keyRelease = event.getIf<sf::Event::KeyReleased>()) {
    if (keyRelease->code == sf::Keyboard::Key::R) {
      this->reset();
    }
  }
}


void ButtonGrid::colorQuadBelowCursor(sf::RenderWindow& window, sf::Color color) {
  sf::Vector2i pixelCoords(sf::Mouse::getPosition(window));
  sf::Vector2f viewMousePos(window.mapPixelToCoords(pixelCoords));

  sf::FloatRect gridBounds = triaVertices.getBounds();
  auto startPos = gridBounds.position;
  size_t row = static_cast<size_t>((-startPos.y + viewMousePos.y) / (cellSize));
  size_t col = static_cast<size_t>((-startPos.x + viewMousePos.x) / (cellSize));

  if (!quadColored(row,col) && gridBounds.contains(viewMousePos)) {
    this->updateCell(row, col, color);
    quadColored(row,col) = true;
  }
}

void ButtonGrid::eraseColorBelowCursor(sf::RenderWindow& window) {
  sf::Vector2i pixelCoords(sf::Mouse::getPosition(window));
  sf::Vector2f viewMousePos(window.mapPixelToCoords(pixelCoords));

  sf::FloatRect gridBounds = triaVertices.getBounds();
  auto startPos = gridBounds.position;
  size_t row = static_cast<size_t>((-startPos.y + viewMousePos.y) / (cellSize));
  size_t col = static_cast<size_t>((-startPos.x + viewMousePos.x) / (cellSize));

  if (quadColored(row,col) && gridBounds.contains(viewMousePos)) {
    this->updateCell(row, col, sf::Color::White);
    quadColored(row,col) = false;
  }
}

void ButtonGrid::setPosition(sf::Vector2f target) {
  position = target;
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < columns; ++c) {
      size_t idx = ((r * columns) + c) * 6;

      for(size_t i = 0; i < 6; ++i) {
        triaVertices[idx + i].position += target;
      }
    }
  }
}

void ButtonGrid::initVertices() {
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < columns; ++c) {

      sf::Vector2f triaPos(position.x + (c * cellSize), position.y + (r * cellSize));
      size_t idx = ((r * columns) + c) * 6;

      triaVertices[idx].position = triaPos;
      triaVertices[idx + 1].position = triaPos + sf::Vector2f(cellSize, 0);
      triaVertices[idx + 2].position = triaPos + sf::Vector2f(cellSize, cellSize);

      triaVertices[idx + 3].position = triaPos;
      triaVertices[idx + 4].position = triaPos + sf::Vector2f(cellSize, cellSize);
      triaVertices[idx + 5].position = triaPos + sf::Vector2f(0, cellSize);

      for (size_t i = 0; i < 6; ++i) {
        triaVertices[idx + i].color = sf::Color::White;
      }
    }
  }
}

void ButtonGrid::reset() {
  this->initVertices();
  quadColored = lin::Matrix<bool>(rows, columns, false);
}

}
