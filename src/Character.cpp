#include "Character.hpp"

Character::Character(){
    this->shape.setSize(sf::Vector2f(50.f, 50.f));
    this->movementSpeed = 200.f;
}

Character::~Character(){

}

void Character::update(const float& dt){
 
}

void Character::render(sf::RenderTarget* window){
    window->draw(this->shape);
}

void Character::move(const float& dt, const float dirX, const float dirY){
    this->shape.move(dirX * this->movementSpeed * dt, dirY * this->movementSpeed * dt);
}