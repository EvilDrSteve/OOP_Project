#include "Table.hpp"

Table::Table(sf::Vector2f pos, int gridSize, bool big) {
    this->occupied = false;
    this->position = sf::Vector2f(pos.x * gridSize, pos.y * gridSize);
    if (big)
        this->size = sf::Vector2f(2.f * gridSize, 3.f * gridSize);
    else
        this->size = sf::Vector2f(2.f * gridSize, 2.f * gridSize);
    this->gridSize = gridSize;
    this->shape = sf::RectangleShape(this->size);
    this->shape.setFillColor(sf::Color(96, 59, 42));
    this->shape.setPosition(this->position);
}

std::vector<sf::Vector2f> Table::getOccupiedTiles() const {
    std::vector<sf::Vector2f> tiles;
    for(int y = this->position.y; y < this->position.y + this->size.y; y += this->gridSize){
        for(int x = this->position.x; x < this->position.x + this->size.x; x += this->gridSize){
            tiles.push_back(sf::Vector2f(y / this->gridSize, x / this->gridSize));
        }
    }

    return tiles;
}

void Table::update(const float& dt) {}

void Table::render(sf::RenderTarget* window) { 
    // window->draw(this->shape);
 }
