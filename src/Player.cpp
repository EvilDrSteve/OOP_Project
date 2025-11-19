#include "Player.hpp"
#include <cmath>

Player::Player(int x, int y, int gridSize){
    this->shape.setOrigin(gridSize/2, gridSize + gridSize / 2);  // (width/2, height)

    this->movementSpeed = 300.f; // pixels per second
    this->currentStep = 0;

    float pixelX = x * gridSize + gridSize / 2.0f;
    float pixelY = y * gridSize + gridSize / 2.0f;

    this->shape.setSize(sf::Vector2f(gridSize, gridSize * 2));
    this->shape.setPosition(pixelX, pixelY);

    this->shape.setFillColor(sf::Color::Green);
}

Player::~Player() {}

void Player::move(const float& dt, const float x, const float y) {
    this->shape.move(x * this->movementSpeed * dt, y * this->movementSpeed * dt);
}

void Player::setPath(const std::vector<sf::Vector2f>& path) {
    this->currentPath = path;
    this->currentStep = 0;
}

bool Player::isMoving() const {
    return this->currentStep < this->currentPath.size();
}

void Player::update(const float& dt) {
    if (this->currentStep < this->currentPath.size()) {
        sf::Vector2f pos = this->shape.getPosition();
        sf::Vector2f target = this->currentPath[currentStep];

        sf::Vector2f diff = (target) - (pos);

        float distanceToTarget = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        

        float moveDistance = movementSpeed * dt;
        if(moveDistance >= distanceToTarget){
            this->shape.setPosition(target);
            currentStep++;
        }
        else if (distanceToTarget > 1.0f) {
            // Move towards target
            float moveX = (diff.x / distanceToTarget) * movementSpeed * dt;
            float moveY = (diff.y / distanceToTarget) * movementSpeed * dt;
            this->shape.move(moveX, moveY);
        }
    }
}