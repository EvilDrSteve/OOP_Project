#include "Sink.hpp"

Sink::Sink(sf::Vector2f gridPos, int gridSize) {
    this->gridSize = gridSize;
    this->position = gridPos;
    this->size = sf::Vector2f(2.f, 3.f);  // 2 tiles wide, 1 tile deep
    this->dishesWashed = 0;
    
    // Setup visual shape
    this->shape = sf::RectangleShape(sf::Vector2f(size.x * gridSize, size.y * gridSize));
    this->shape.setPosition(position.x * gridSize, position.y * gridSize);
    this->shape.setFillColor(sf::Color(180, 180, 200));  // Light gray/blue for sink
    this->shape.setOutlineColor(sf::Color(100, 100, 120));
    this->shape.setOutlineThickness(2.f);
    
    this->texture.loadFromFile("assets/sink.png");
    this->sprite.setTexture(this->texture);
    this->sprite.setPosition(this->position.x * this->gridSize, this->position.y * this->gridSize);
    this->sprite.setScale(0.6, 0.6);
    
    //Animation
    this->frameTimer = 0;
    this->currentFrame = 0;
    this->frameTime = 0.07f;

}

void Sink::dropDishes() {
    dishesWashed++;
}

void Sink::update(const float& dt) {

    //Animate the sink
    frameTimer += dt;
    if (frameTimer >= frameTime)
    {
        frameTimer = 0.f;
        currentFrame++;
        if (currentFrame >= 12)
        {
            currentFrame = 0;
        }
        sprite.setTextureRect(sf::IntRect(96 * (currentFrame % 3), 144 * (currentFrame / 4), 96, 144));

        this->sprite.setPosition(this->position.x * this->gridSize, this->position.y * this->gridSize);
    }
}

void Sink::render(sf::RenderTarget* window) {
    window->draw(sprite);

}

sf::FloatRect Sink::getBounds() const {
    return shape.getGlobalBounds();
}

bool Sink::containsPoint(sf::Vector2f point) const {
    return getBounds().contains(point);
}

// Return which tiles the sink occupies
std::vector<sf::Vector2f> Sink::getOccupiedTiles() const {
    std::vector<sf::Vector2f> tiles;
    for (int y = 0; y < (int)size.y; y++) {
        for (int x = 0; x < (int)size.x; x++) {
            tiles.push_back(sf::Vector2f(position.x + x, position.y + y));
        }
    }
    return tiles;
}

sf::Vector2f Sink::getInteractionPoint() const {
    // Player stands in front of the sink (one tile below)
    return sf::Vector2f(position.x + size.x / 2.f, position.y + size.y);
}