#include "Character.hpp"

Character::Character(int x, int y, int width, int height){
    this->shape.setSize(sf::Vector2f(width, height));
    this->position = sf::Vector2f(x, y);
}

Character::~Character(){

}

void Character::render(sf::RenderTarget* window){
    window->draw(this->shape);
}

sf::Vector2f Character::getPosition() const{
    return this->shape.getPosition();
}