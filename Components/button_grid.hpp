#pragma once
#include <SFML/Graphics.hpp>
#include <matrix.hpp>
#include <vector>
#include "vector.hpp"

namespace components {

class ButtonGrid {
public:
    ButtonGrid(size_t rows, size_t columns);
    size_t rows;
    size_t columns;
    float cellSize = 50.0F;
    sf::Vector2f position;

    sf::VertexArray triaVertices;
    lin::Matrix<float> quadColored;

    void setPosition(sf::Vector2f target);
    void setCellSize(float size);
    void colorCell(size_t row, size_t col, sf::Color color);
    void interpolateSurroundingCells(size_t row, size_t col, sf::Vector2f mousePos);
    size_t getVertexArrayIndex(size_t row, size_t col) const;
    sf::Vector2f getTrueCellPosition(size_t row, size_t col) const;

    void updateStatus(sf::RenderWindow& window, const sf::Event& event);
    void drawBelowCursor(sf::RenderWindow& window);
    void eraseColorBelowCursor(sf::RenderWindow& window);
    void initVertices();
    void reset();

    //lin::Vector<float> getNormalizedInput();
};
}
