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
    lin::Matrix<bool> quadColored;

    void setPosition(sf::Vector2f target);
    void setCellSize(float size);
    void updateCell(size_t row, size_t col, sf::Color color);
    void updateStatus(sf::RenderWindow& window, const sf::Event& event);
    void colorQuadBelowCursor(sf::RenderWindow& window, sf::Color color);
    void eraseColorBelowCursor(sf::RenderWindow& window);
    void initVertices();
    void reset();

    lin::Vector<float> getNormalizedInput();
};
}
