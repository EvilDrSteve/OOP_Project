#include "Player.hpp"
#include "Animation.hpp"
#include "Kitchen.hpp"
#include "Sink.hpp"
#include "Order.hpp"
#include "Customer.hpp"
#include "CustomerState.hpp"

Player::Player(int x, int y, int gridSize, std::string name) 
    : Character("assets/Modern tiles_Free/Characters_free/" + name + "_16x16.png") {
    
    this->movementSpeed = 250.f;
    this->currentStep = 0;
    this->pendingAction = PlayerAction::NONE;
    this->targetTable = nullptr;
    this->targetKitchen = nullptr;
    this->targetSink = nullptr;
    this->selectedTable = nullptr;
    
    float pixelX = x * gridSize + gridSize / 2.0f;
    float pixelY = y * gridSize + gridSize / 2.0f;
    
    this->sprite.setPosition(pixelX, pixelY);
    this->sprite.setScale(sf::Vector2f(gridSize / 12.f, gridSize / 12.f));
    
    initAnimations();
    
    sf::IntRect rect = sprite.getTextureRect();
    sprite.setOrigin(rect.width / 2.f, rect.height);
    
    // Setup inventory UI slots
    inventorySlot1.setSize(sf::Vector2f(24.f, 24.f));
    inventorySlot1.setFillColor(sf::Color(60, 60, 60, 200));
    inventorySlot1.setOutlineColor(sf::Color::White);
    inventorySlot1.setOutlineThickness(1.f);
    
    inventorySlot2.setSize(sf::Vector2f(24.f, 24.f));
    inventorySlot2.setFillColor(sf::Color(60, 60, 60, 200));
    inventorySlot2.setOutlineColor(sf::Color::White);
    inventorySlot2.setOutlineThickness(1.f);
}

Player::~Player() {}

void Player::move(const float& dt, const float x, const float y) {
    this->sprite.move(x * this->movementSpeed * dt, y * this->movementSpeed * dt);
}

void Player::setPath(const std::vector<sf::Vector2f>& path) {
    this->currentPath = path;
    this->currentStep = 0;
    this->pendingAction = PlayerAction::NONE;
}

void Player::setDestination(const std::vector<sf::Vector2f>& path, PlayerAction action) {
    this->currentPath = path;
    this->currentStep = 0;
    this->pendingAction = action;
}

bool Player::isMoving() const {
    return this->currentStep < this->currentPath.size();
}

void Player::update(const float& dt) {
    this->animate(dt);
    
    if (this->currentStep < this->currentPath.size()) {
        sf::Vector2f pos = this->sprite.getPosition();
        sf::Vector2f target = this->currentPath[currentStep];
        
        sf::Vector2f diff = target - pos;
        float distanceToTarget = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        
        float moveDistance = movementSpeed * dt;
        if (moveDistance >= distanceToTarget) {
            this->sprite.setPosition(target);
            currentStep++;
            
            // Check if we've reached the final destination
            if (currentStep >= currentPath.size()) {
                this->currentAction = "idle";
                
                // Execute pending action
                switch (pendingAction) {
                    case PlayerAction::INTERACT_TABLE:
                        if (targetTable) {
                            interactWithTable(targetTable);
                        }
                        break;
                    case PlayerAction::INTERACT_KITCHEN:
                        if (targetKitchen) {
                            interactWithKitchen(targetKitchen);
                        }
                        break;
                    case PlayerAction::INTERACT_SINK:
                        if (targetSink) {
                            interactWithSink(targetSink);
                        }
                        break;
                    default:
                        break;
                }
                
                if (onReachDestination) {
                    onReachDestination();
                }
                
                pendingAction = PlayerAction::NONE;
            }
        } else if (distanceToTarget > 1.0f) {
            float moveX = (diff.x / distanceToTarget) * movementSpeed * dt;
            float moveY = (diff.y / distanceToTarget) * movementSpeed * dt;
            
            if (moveX > 0) this->direction = Direction::RIGHT;
            if (moveX < 0) this->direction = Direction::LEFT;
            if (moveY > 0) this->direction = Direction::DOWN;
            if (moveY < 0) this->direction = Direction::UP;
            
            this->currentAction = "walk";
            this->sprite.move(moveX, moveY);
        }
    }
}

void Player::render(sf::RenderTarget* window) {
    window->draw(this->sprite);
    renderInventoryUI(window);
}

void Player::renderInventoryUI(sf::RenderTarget* window) {
    // Position slots above player's head
    sf::Vector2f playerPos = sprite.getPosition();
    
    inventorySlot1.setPosition(playerPos.x - 26, playerPos.y - 60);
    inventorySlot2.setPosition(playerPos.x + 2, playerPos.y - 60);
    
    // Color slots based on contents
    const Item* item1 = inventory.getSlot(0);
    const Item* item2 = inventory.getSlot(1);
    
    if (item1 && !item1->isEmpty()) {
        switch (item1->type) {
            case ItemType::ORDER:
                inventorySlot1.setFillColor(sf::Color(255, 255, 100, 200)); // Yellow
                break;
            case ItemType::FOOD:
                inventorySlot1.setFillColor(sf::Color(100, 255, 100, 200)); // Green
                break;
            case ItemType::DIRTY_DISHES:
                inventorySlot1.setFillColor(sf::Color(150, 100, 80, 200)); // Brown
                break;
            default:
                inventorySlot1.setFillColor(sf::Color(60, 60, 60, 200));
                break;
        }
    } else {
        inventorySlot1.setFillColor(sf::Color(60, 60, 60, 200));
    }
    
    if (item2 && !item2->isEmpty()) {
        switch (item2->type) {
            case ItemType::ORDER:
                inventorySlot2.setFillColor(sf::Color(255, 255, 100, 200));
                break;
            case ItemType::FOOD:
                inventorySlot2.setFillColor(sf::Color(100, 255, 100, 200));
                break;
            case ItemType::DIRTY_DISHES:
                inventorySlot2.setFillColor(sf::Color(150, 100, 80, 200));
                break;
            default:
                inventorySlot2.setFillColor(sf::Color(60, 60, 60, 200));
                break;
        }
    } else {
        inventorySlot2.setFillColor(sf::Color(60, 60, 60, 200));
    }
    
    window->draw(inventorySlot1);
    window->draw(inventorySlot2);
}

void Player::initAnimations() {
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

sf::Sprite& Player::getSprite() {
    return this->sprite;
}

void Player::interactWithTable(Table* table) {
    // The actual logic is handled in Grid::handleTableInteraction
    this->selectedTable = table;
}

void Player::interactWithKitchen(Kitchen* kitchen) {
    // This will be called by Grid which handles the actual order logic
    this->targetKitchen = nullptr;
}

void Player::interactWithSink(Sink* sink) {
    // Drop any dirty dishes
    if (inventory.hasItemType(ItemType::DIRTY_DISHES)) {
        inventory.removeItemOfType(ItemType::DIRTY_DISHES);
        sink->dropDishes();
    }
    this->targetSink = nullptr;
}