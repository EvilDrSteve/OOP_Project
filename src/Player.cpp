#include "Player.hpp"
#include "Animation.hpp"

Player::Player(int x, int y, int gridSize, std::string name) : Character("assets/Modern tiles_Free/Characters_free/" + name + "_16x16.png"){
    
    this->movementSpeed = 100.f; // pixels per second
    this->currentStep = 0;
    
    float pixelX = x * gridSize + gridSize / 2.0f;
    float pixelY = y * gridSize + gridSize / 2.0f;
    
    this->sprite.setPosition(pixelX, pixelY);
    this->sprite.setScale(sf::Vector2f(gridSize / 12.f, gridSize / 12.f));
    
    initAnimations();
    // this->addAnimation("idle", 4, 16, 32, 0);
    // After animation sets the sprite's textureRect:
    sf::IntRect rect = sprite.getTextureRect();
    sprite.setOrigin(rect.width / 2.f, rect.height);

}

Player::~Player() {}

void Player::move(const float& dt, const float x, const float y) {

    this->sprite.move(x * this->movementSpeed * dt, y * this->movementSpeed * dt);

}   

void Player::setPath(const std::vector<sf::Vector2f>& path) {
    this->currentPath = path;
    this->currentStep = 0;
}

bool Player::isMoving() const {
    return this->currentStep < this->currentPath.size();
}

void Player::update(const float& dt) {
    
    this->animate(dt);
    
    if (this->currentStep < this->currentPath.size()) {
        sf::Vector2f pos = this->sprite.getPosition();
        sf::Vector2f target = this->currentPath[currentStep];
        
        sf::Vector2f diff = (target) - (pos);
        
        float distanceToTarget = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        
        
        float moveDistance = movementSpeed * dt;
        if(moveDistance >= distanceToTarget){
            this->sprite.setPosition(target);
            currentStep++;

            this->currentAction = "idle";
        }
        else if (distanceToTarget > 1.0f) {
            // Move towards target
            float moveX = (diff.x / distanceToTarget) * movementSpeed * dt;
            float moveY = (diff.y / distanceToTarget) * movementSpeed * dt;
            if(moveX > 0) this->direction = Direction::RIGHT;
            if(moveX < 0) this->direction = Direction::LEFT;
            if(moveY > 0) this->direction = Direction::DOWN;
            if(moveY < 0) this->direction = Direction::UP;
            this->currentAction = "walk";
            this->sprite.move(moveX, moveY);
        }
    }
}
// 02199071550
void Player::render(sf::RenderTarget* window){
    window->draw(this->sprite);
}

void Player::initAnimations(){
    this->addAnimation("idle0", 0, 1, 6, 16, 32);
    this->addAnimation("idle1", 6, 1, 6, 16, 32);
    this->addAnimation("idle2", 12, 1, 6, 16, 32);
    this->addAnimation("idle3", 18, 1, 6, 16, 32);
    this->addAnimation("walk0", 0, 2, 6, 16, 32);
    this->addAnimation("walk1", 6, 2, 6, 16, 32);
    this->addAnimation("walk2", 12, 2, 6, 16, 32);
    this->addAnimation("walk3", 18, 2, 6, 16, 32);
    this->sprite.setTextureRect(animations[currentAction + std::to_string(this->direction)][currentFrame]);

}

sf::Sprite& Player::getSprite(){
    return this->sprite;
}