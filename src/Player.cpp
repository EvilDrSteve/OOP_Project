#include "Player.hpp"

Player::Player(int x, int y) : Character(x, y, 16, 32){
    this->movementSpeed = 100;
}

Player::~Player(){

}

void Player::move(const float& dt, const float dirX, const float dirY){
    this->shape.move(dirX * this->movementSpeed * dt, dirY * this->movementSpeed * dt);
}

void Player::update(const float& dt){

}

