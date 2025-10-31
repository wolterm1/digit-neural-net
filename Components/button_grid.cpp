// button_grid.cpp
#include "button_grid.hpp"
#include <cmath>
#include "vector.hpp"

namespace components {

ButtonGrid::ButtonGrid(size_t rows, size_t columns) : rows(rows), columns(columns) {
  // grid contains rectangles made out of 2 Triangles with 3 vertices each;
  triaVertices = sf::VertexArray(sf::PrimitiveType::Triangles, rows * columns * 6);
  quadColored = lin::Matrix<float>(rows, columns, 0.0);
  initVertices();
}


void ButtonGrid::setCellSize(float size) {
  cellSize = size;
  initVertices();
}

void ButtonGrid::colorCell(size_t row, size_t col, sf::Color color) {
  std::cout << "Updating cell : " << row << ", " << col << '\n';
  size_t idx = getVertexArrayIndex(row, col); getVertexArrayIndex(row, col); 
    for (size_t i = 0; i < 6; ++i) {
      triaVertices[idx + i].color = color;
    }
}

size_t ButtonGrid::getVertexArrayIndex(size_t row, size_t col) const {
  return (row * columns + col) * 6;
}

void ButtonGrid::updateStatus(sf::RenderWindow& window, const sf::Event& event) {
  if(const auto& mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
    if(mousePress->button == sf::Mouse::Button::Left) {
      drawBelowCursor(window);
    } else if (mousePress->button == sf::Mouse::Button::Right) {
      eraseColorBelowCursor(window);
    }
  }
  else if (const auto& mouseMove = event.getIf<sf::Event::MouseMoved>()) {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      drawBelowCursor(window);
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


void ButtonGrid::drawBelowCursor(sf::RenderWindow& window) {
  sf::Vector2i pixelCoords(sf::Mouse::getPosition(window));
  sf::Vector2f viewMousePos(window.mapPixelToCoords(pixelCoords));

  sf::FloatRect gridBounds = triaVertices.getBounds();
  auto startPos = gridBounds.position;
  size_t row = static_cast<size_t>((-startPos.y + viewMousePos.y) / (cellSize));
  size_t col = static_cast<size_t>((-startPos.x + viewMousePos.x) / (cellSize));

  if (gridBounds.contains(viewMousePos)) {
    this->colorCell(row, col, sf::Color::White);
    this->interpolateSurroundingCells(row, col, viewMousePos);
  }
}

void ButtonGrid::interpolateSurroundingCells(size_t row, size_t col, sf::Vector2f mousePos) {
  sf::Vector2f origin = getTrueCellPosition(row, col);
  for (int r = -1; r <= 1; ++r) {
    for (int c = -1; c <= 1; ++c) {
      if (getVertexArrayIndex(row + r, col + c) < triaVertices.getVertexCount() && (r != 0 || c != 0)) {
        sf::Vector2f  neighbourCenter = getTrueCellPosition(row + r, col + c);
        size_t idx = getVertexArrayIndex(r, c);
        float distance = std::sqrt((origin.x - neighbourCenter.x) * (origin.x - neighbourCenter.x) + (origin.y - neighbourCenter.y) * (origin.y - neighbourCenter.y));
        colorCell(row + r, col + c, sf::Color(100,100,distance*2, 255));
      }
    }
  }
}

sf::Vector2f ButtonGrid::getTrueCellPosition(size_t row, size_t col) const {
  size_t idx = getVertexArrayIndex(row, col);
  sf::Vector2f offset(cellSize/2.0F, cellSize/2.0F);
  return triaVertices[idx].position + offset;
}

void ButtonGrid::eraseColorBelowCursor(sf::RenderWindow& window) {
  sf::Vector2i pixelCoords(sf::Mouse::getPosition(window));
  sf::Vector2f viewMousePos(window.mapPixelToCoords(pixelCoords));

  sf::FloatRect gridBounds = triaVertices.getBounds();
  auto startPos = gridBounds.position;
  size_t row = static_cast<size_t>((-startPos.y + viewMousePos.y) / (cellSize));
  size_t col = static_cast<size_t>((-startPos.x + viewMousePos.x) / (cellSize));

  if (gridBounds.contains(viewMousePos)) {
    this->colorCell(row, col, sf::Color::Black);
  }
}

void ButtonGrid::setPosition(sf::Vector2f target) {
  position = target;
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < columns; ++c) {
      size_t idx = getVertexArrayIndex(r, c);

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
      size_t idx = getVertexArrayIndex(r, c);

      triaVertices[idx].position = triaPos;
      triaVertices[idx + 1].position = triaPos + sf::Vector2f(cellSize, 0);
      triaVertices[idx + 2].position = triaPos + sf::Vector2f(cellSize, cellSize);

      triaVertices[idx + 3].position = triaPos;
      triaVertices[idx + 4].position = triaPos + sf::Vector2f(cellSize, cellSize);
      triaVertices[idx + 5].position = triaPos + sf::Vector2f(0, cellSize);

      for (size_t i = 0; i < 6; ++i) {
        triaVertices[idx + i].color = sf::Color::Black;
      }
    }
  }
}

void ButtonGrid::reset() {
  this->initVertices();
}

//lin::Vector<float> ButtonGrid::getNormalizedInput() {
//  lin::Vector<float> result(rows * columns);
//  size_t idx = 0;
//  for (auto &colored : quadColored) {
//    if (colored) {
//      result[idx] = 1.0F;
//    } else {
//      result[idx] = -1.0F;
//    }
//    ++idx;
//  }
//  return result;
//}

}
