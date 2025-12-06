#include "Chef.hpp"
#include <cmath>
#include "Grid.hpp"

Chef::Chef(int x, int y, int gridSize, std::string name)
    : Character("assets/Modern tiles_Free/Characters_free/chef.png")
{
    //Initialize chef attributes

    this->movementSpeed = 100.f; 
    this->currentStep = 0;
    
    float pixelX = x * gridSize + gridSize / 2.f;
    float pixelY = y * gridSize + gridSize / 2.f;
    
    this->sprite.setPosition(pixelX, pixelY);
    this->sprite.setScale(sf::Vector2f(gridSize / 12.f, gridSize / 12.f));
    
    initAnimations();

    sf::IntRect rect = sprite.getTextureRect();
    sprite.setOrigin(rect.width / 2.f, rect.height);
}

Chef::~Chef() {}


void Chef::update(const float& dt) {
    this->animate(dt);
}

void Chef::render(sf::RenderTarget* window) {
    window->draw(this->sprite);
   
}

void Chef::initAnimations() {
    //Load animations
    this->addAnimation("idle0", 18, 1, 6, 16, 32);
    this->addAnimation("idle1", 6, 1, 6, 16, 32);
    this->addAnimation("idle2", 12, 1, 6, 16, 32);
    this->addAnimation("idle3", 0, 1, 6, 16, 32);
    this->addAnimation("walk0", 0, 2, 6, 16, 32);
    this->addAnimation("walk1", 6, 2, 6, 16, 32);
    this->addAnimation("walk2", 12, 2, 6, 16, 32);
    this->addAnimation("walk3", 18, 2, 6, 16, 32);
}

void Chef::move(const float& dt, const float x, const float y) {
// Chef does not move
}

void Chef::setSpritePosition(const sf::Vector2f& pos) {
    this->sprite.setPosition(pos);
}

sf::Vector2f Chef::getSize() const {
    sf::FloatRect bounds = this->sprite.getGlobalBounds();
    return sf::Vector2f(bounds.width, bounds.height);
}
