#include "Table.hpp"

Table::Table(sf::Vector2f pos, int gridSize, bool big) {
    this->occupied = false;
    this->bigTable = big;
    this->position = sf::Vector2f(pos.x * gridSize, pos.y * gridSize);
    if (big)
        this->size = sf::Vector2f(4.f * gridSize, 4.f * gridSize);
    else
        this->size = sf::Vector2f(4.f * gridSize, 3.f * gridSize);
    
    this->gridSize = gridSize;
    this->shape = sf::RectangleShape(this->size);
    this->shape.setFillColor(sf::Color(96, 59, 42));
    this->shape.setPosition(this->position);

    if(this->bigTable){
        this->chairTexture.loadFromFile("assets/Table-Big-Chairs.png");
        this->tableTexture.loadFromFile("assets/Table-Big.png");
    }else {
        this->chairTexture.loadFromFile("assets/Table-Small-Chairs.png");
        this->tableTexture.loadFromFile("assets/Table-Small.png");
    }
    this->tableSprite.setTexture(this->tableTexture);
    this->tableSprite.setOrigin(sf::Vector2f(8.f, 11.f));
    this->tableSprite.setPosition(this->position);
    this->tableSprite.setScale(sf::Vector2f(this->size.x / 48, this->size.y / (this->bigTable ? 48 : 32)));

}

std::vector<sf::Vector2f> Table::getOccupiedTiles() const {
    std::vector<sf::Vector2f> tiles;
    for(int y = this->position.y; y < this->position.y + this->size.y; y += this->gridSize){
        for(int x = this->position.x; x < this->position.x + this->size.x; x += this->gridSize){
            tiles.push_back(sf::Vector2f(x / this->gridSize, y / this->gridSize));
        }
    }

    if(this->bigTable){
        tiles.push_back(sf::Vector2f((this->position.x / this->gridSize) - 1, (this->position.y / this->gridSize) + 1));
        tiles.push_back(sf::Vector2f((this->position.x / this->gridSize) - 1, (this->position.y / this->gridSize) + 2));
        tiles.push_back(sf::Vector2f(((this->position.x + this->size.x) / this->gridSize), (this->position.y / this->gridSize) + 1));
        tiles.push_back(sf::Vector2f(((this->position.x + this->size.x) / this->gridSize), (this->position.y / this->gridSize) + 2));
    }else {
        tiles.push_back(sf::Vector2f((this->position.x / this->gridSize) - 1, (this->position.y / this->gridSize) + 1));
        tiles.push_back(sf::Vector2f(((this->position.x + this->size.x) / this->gridSize), (this->position.y / this->gridSize) + 1));
    }
    return tiles;
}

void Table::update(const float& dt) {}

void Table::render(sf::RenderTarget* window) { 
    this->tableSprite.setTexture(chairTexture);
    window->draw(this->tableSprite);
}

void Table::lateRender(sf::RenderTarget* window){
    this->tableSprite.setTexture(tableTexture);
    window->draw(this->tableSprite);
 }

sf::FloatRect Table::getBounds() const{
    return this->tableSprite.getGlobalBounds();
}

int Table::getSize() const {
    return this->bigTable ? 4 : 2;
}

void Table::seatCustomer() {
    this->occupied = true;
}

bool Table::getOccopied() const{
    return this->occupied;
}

sf::Vector2f Table::getPosition() const{
    return sf::Vector2f((this->tableSprite.getPosition().x / this->gridSize), (this->tableSprite.getPosition().y / this->gridSize) + 2);
}