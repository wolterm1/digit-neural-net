#pragma once
#include <SFML/Graphics.hpp>
#include <matrix.hpp>
#include <vector>
#include "vector.hpp"

namespace components {

class ButtonGrid : public sf::Drawable{
public:
    ButtonGrid(size_t rows, size_t columns);
    size_t rows;
    size_t columns;
    float size = 100.0F;

    sf::VertexArray triaVertices;
    lin::Matrix<uint8_t> quadColors;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setPosition(sf::Vector2f target);
    void setSize(float target);
    float getSize() const;
    void colorCell(size_t row, size_t col, sf::Color color);
    void interpolateSurroundingCells(size_t row, size_t col, sf::Vector2f mousePos);
    size_t getVertexArrayIndex(size_t row, size_t col) const;
    sf::Vector2f getTrueCellPosition(size_t row, size_t col) const;

    lin::Vector<uint8_t> getImage();
    void updateStatus(sf::RenderWindow& window, const sf::Event& event);
    void drawBelowCursor(sf::RenderWindow& window);
    void eraseColorBelowCursor(sf::RenderWindow& window);
    void initVertices();
    void reset();
    float getCellSize() const;
    sf::Vector2f getPosition() const;


private:
  sf::Vector2f position;
    //lin::Vector<float> getNormalizedInput();
};
}
