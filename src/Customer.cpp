#include "Customer.hpp"
#include "Table.hpp"
#include <cstdlib>

Customer::Customer(int size, int gridSize) {
    this->groupSize = size;
    this->position = sf::Vector2f(4.f, 4.f);
    this->gridSize = gridSize;
    this->occupiesTable = nullptr;
    this->isDragging = false;
    
    this->state = CustomerState::IN_QUEUE;
    this->stateTimer = 0.f;
    this->patienceTimer = 60.f;  // 60 seconds of patience
    this->maxPatience = 60.f;
    
    this->menuReadTime = 3.f + (rand() % 30) / 10.f;   // 3-6 seconds
    this->eatingTime = 5.f + (rand() % 50) / 10.f;     // 5-10 seconds
    this->postEatingTime = 2.f + (rand() % 20) / 10.f; // 2-4 seconds
    
    // Visual
    this->showIndicator = false;
    this->currentIndicator = CustomerIndicator::NONE;
    
    this->indicatorTexture.loadFromFile("assets/emotes.png");
    this->indicatorSprite.setTexture(this->indicatorTexture);

    this->plateTexture.loadFromFile("assets/plates.png");
    this->plateSprite.setTexture(plateTexture);

    this->plateSprite.setScale(this->gridSize / 24.f, this->gridSize / 24.f);
    // Dishes
    this->hasDirtyDishes = false;
    
    this->billAmount = groupSize * (15 + rand() % 10);  // $15-25 per person
    this->tipAmount = 0;
    
    this->setupCharacters();
}

Customer::~Customer() {
    for (CustomerCharacter* character : this->characters) {
        delete character;
    }
    this->characters.clear();
}

void Customer::setupCharacters() {
    std::vector<std::string> characterFiles = {
        "assets/Modern tiles_Free/Characters_free/Adam_16x16.png",
        "assets/Modern tiles_Free/Characters_free/Alex_16x16.png",
        "assets/Modern tiles_Free/Characters_free/Amelia_16x16.png",
        "assets/Modern tiles_Free/Characters_free/Bob_16x16.png"
    };

    for (int i = 0; i < this->groupSize; i++) {
        int randomIndex = rand() % characterFiles.size();
        CustomerCharacter* character = 
            new CustomerCharacter(characterFiles[randomIndex], this->gridSize);

        character->addAnimation("idle0", 0, 1, 6, 16, 32);
        character->addAnimation("idle1", 6, 1, 6, 16, 32);
        character->addAnimation("idle2", 12, 1, 6, 16, 32);
        character->addAnimation("idle3", 18, 1, 6, 16, 32);
        character->addAnimation("sitting0", 0, 4, 6, 32, 32);
        character->addAnimation("sitting1", 6, 4, 6, 32, 32);
        character->addAnimation("sitting2", 0, 4, 6, 32, 32);
        character->addAnimation("sitting3", 6, 4, 6, 32, 32);

        character->setAction("idle");
        character->setSpriteScale(this->gridSize);
        characters.push_back(character);
    }

    this->updateCharacterPositions();
}

void Customer::updateCharacterPositions() {
    sf::Vector2f offset(0.75, 0.75);
    
    if (this->occupiesTable) {
        if (groupSize == 2) {
            characters[0]->setPosition(
                sf::Vector2f(this->position.x - 0.35, this->position.y - 0.6));
            characters[1]->setPosition(
                sf::Vector2f(this->position.x + 3.35, this->position.y - 0.6));
        } else if (groupSize == 4) {
            characters[0]->setPosition(
                sf::Vector2f(this->position.x - 0.35, this->position.y - offset.y));
            characters[1]->setPosition(
                sf::Vector2f(this->position.x + 3.35, this->position.y - offset.y));
            characters[2]->setPosition(
                sf::Vector2f(this->position.x - 0.35, this->position.y + offset.y));
            characters[3]->setPosition(
                sf::Vector2f(this->position.x + 3.35, this->position.y + offset.y));
        }
    } else {
        if (groupSize == 2) {
            characters[0]->setPosition(
                sf::Vector2f(this->position.x - offset.x, this->position.y));
            characters[1]->setPosition(
                sf::Vector2f(this->position.x + offset.x, this->position.y));
        } else if (groupSize == 4) {
            characters[0]->setPosition(
                sf::Vector2f(this->position.x - offset.x, this->position.y - offset.y));
            characters[1]->setPosition(
                sf::Vector2f(this->position.x + offset.x, this->position.y - offset.y));
            characters[2]->setPosition(
                sf::Vector2f(this->position.x - offset.x, this->position.y + offset.y));
            characters[3]->setPosition(
                sf::Vector2f(this->position.x + offset.x, this->position.y + offset.y));
        }
    }
}

void Customer::setState(CustomerState newState) {
    this->state = newState;
    this->stateTimer = 0.f;
    updateIndicator();
}

void Customer::updateIndicator() {
    switch (state) {
        case CustomerState::READY_TO_ORDER:
            currentIndicator = CustomerIndicator::READY_TO_ORDER;
            showIndicator = true;
            break;
        case CustomerState::ORDER_TAKEN:
            currentIndicator = CustomerIndicator::WAITING_FOR_FOOD;
            showIndicator = true;
            break;
        case CustomerState::READY_TO_PAY:
            currentIndicator = CustomerIndicator::READY_TO_PAY;
            showIndicator = true;
            break;
        case CustomerState::FOOD_DELIVERED:
            currentIndicator = CustomerIndicator::EATING;
            showIndicator = true;
            break;
        default:
            showIndicator = false;
    }
    
    // Override with angry indicator if patience is low
    if (patienceTimer < maxPatience * 0.25f && state != CustomerState::FOOD_DELIVERED && 
        state != CustomerState::PAID && state != CustomerState::LEFT) {
        currentIndicator = CustomerIndicator::ANGRY;
        showIndicator = true;
    }
}

void Customer::update(const float& dt) {
    // Update characters
    for (CustomerCharacter* character : this->characters) {
        character->update(dt);
    }
    
    // Update state timer
    stateTimer += dt;
    
    switch (state) {
        case CustomerState::IN_QUEUE:
            patienceTimer -= dt * 0.5f;  // Slower patience drain in queue
            break;
            
        case CustomerState::SEATED:
            // Reading menu
            if (stateTimer >= menuReadTime) {
                setState(CustomerState::READY_TO_ORDER);
            }
            break;
            
        case CustomerState::READY_TO_ORDER:
            // Waiting for player to take order
            patienceTimer -= dt;
            break;
            
        case CustomerState::ORDER_TAKEN:
            // Waiting for food
            patienceTimer -= dt;
            break;
            
        case CustomerState::FOOD_DELIVERED:
            // Eating
            if (stateTimer >= eatingTime) {
                setState(CustomerState::READY_TO_PAY);
            }
            break;
            
        case CustomerState::READY_TO_PAY:
            patienceTimer -= dt * 0.3f;  // Slower drain after eating
            hasDirtyDishes = true;
            break;
            
        case CustomerState::PAID:
            // Customer leaving, dishes remain
            setState(CustomerState::LEFT);
            break;
            
        case CustomerState::LEFT:
            break;
            
        default:
            break;
    }
    
    updateIndicator();
    
    if (patienceTimer < 0) patienceTimer = 0;
}

void Customer::render(sf::RenderTarget* window) {
    for (CustomerCharacter* character : this->characters) {
        character->render(window);
    }
}
void Customer::lateRender(sf::RenderTarget* window) {

    this->renderIndicator(window);

        // Draw indicator above the table
    sf::Vector2f tablePos(position.x * gridSize, position.y * gridSize);
    bool renderPlates = false;
    if (this->currentIndicator == CustomerIndicator::EATING) {
        this->plateSprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
        renderPlates = true;
    } else if (this->hasDirtyDishes) {
        this->plateSprite.setTextureRect(sf::IntRect(32 * 2, 0, 32, 32));
        renderPlates = true;
    }

    if (renderPlates) {
        this->plateSprite.setPosition(tablePos.x + (10 / 24.f * this->gridSize), tablePos.y - (45 / 24.f * this->gridSize));
        window->draw(this->plateSprite);
        this->plateSprite.setPosition(tablePos.x + (52 / 24.f * this->gridSize), tablePos.y - (45 / 24.f * this->gridSize));
        window->draw(this->plateSprite);
        
        if (this->getTable()->getSize() == 4) {
            this->plateSprite.setPosition(tablePos.x + (10 / 24.f * this->gridSize), tablePos.y - (15 / 24.f * this->gridSize));
            window->draw(this->plateSprite);
            this->plateSprite.setPosition(tablePos.x + (52 / 24.f * this->gridSize), tablePos.y - (15 / 24.f * this->gridSize));
            window->draw(this->plateSprite);
        }
    }
}

void Customer::renderIndicator(sf::RenderTarget* window) {
    if (!occupiesTable || !this->showIndicator) return;
    
    // Draw indicator above the table
    sf::Vector2f tablePos(position.x * gridSize, position.y * gridSize);
    
    
    switch (currentIndicator) {
        case CustomerIndicator::READY_TO_ORDER:
            this->indicatorSprite.setTextureRect(sf::IntRect(5 * 16, 0, 16, 16));
            break;
            case CustomerIndicator::WAITING_FOR_FOOD:
            this->indicatorSprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
            break;
            case CustomerIndicator::READY_TO_PAY:
            this->indicatorSprite.setTextureRect(sf::IntRect(17 * 16, 0, 16, 16));
            break;
            case CustomerIndicator::ANGRY:
            this->indicatorSprite.setTextureRect(sf::IntRect(23 * 16, 0, 16, 16));
            break;
        default:
            return;
    }
    this->indicatorSprite.setPosition(tablePos.x + (90 / 24.f * this->gridSize), tablePos.y - (70 / 24.f * this->gridSize));
    this->indicatorSprite.setScale(this->gridSize / 12.f, this->gridSize / 12.f);
    window->draw(this->indicatorSprite);
}

sf::Vector2f Customer::getPosition() const {
    return this->characters[0]->getPosition();
}

void Customer::setPosition(sf::Vector2f pos) {
    this->position = pos;
    updateCharacterPositions();
}

std::vector<CustomerCharacter*> Customer::getCharacters() const {
    return this->characters;
}

bool Customer::containsPoint(sf::Vector2f point) const {
    for (CustomerCharacter* character : characters) {
        sf::FloatRect bounds = character->getSprite().getGlobalBounds();
        if (bounds.contains(point)) {
            return true;
        }
    }
    return false;
}

void Customer::startDrag(sf::Vector2f mouseWorldPos) {
    isDragging = true;
    dragStartPosition = this->position;
    
    sf::Vector2f worldPos(this->position.x * this->gridSize, 
                          this->position.y * this->gridSize);
    dragOffset = worldPos - mouseWorldPos;
    
    for (CustomerCharacter* character : characters) {
        sf::Sprite& sprite = character->getSprite();
        sprite.setColor(sf::Color(255, 255, 255, 180));
    }
}

void Customer::drag(sf::Vector2f mouseWorldPos) {
    if (!isDragging) return;
    
    sf::Vector2f newWorldPos = mouseWorldPos + dragOffset;
    this->setPosition(sf::Vector2f(newWorldPos.x / this->gridSize, 
                                   newWorldPos.y / this->gridSize));
}

void Customer::stopDrag() {
    isDragging = false;
    
    for (CustomerCharacter* character : characters) {
        sf::Sprite& sprite = character->getSprite();
        sprite.setColor(sf::Color::White);
    }
}

void Customer::returnToStartPosition() {
    this->setPosition(this->dragStartPosition);
}

bool Customer::getIsDragging() const {
    return this->isDragging;
}

int Customer::getGroupSize() const {
    return this->groupSize;
}

void Customer::sitAtTable(Table* table) {
    this->occupiesTable = table;
    this->occupiesTable->seatCustomer();
    sf::Vector2f tablePos = occupiesTable->getPosition();
    
    this->setPosition(tablePos);
    this->setAction("sitting");
    
    // Start seated state
    setState(CustomerState::SEATED);
}

void Customer::setAction(const std::string& action) {
    for (size_t i = 0; i < this->characters.size(); i++) {
        this->characters[i]->setAction(action);
        this->characters[i]->setDirection((Direction)(i % 2));
    }
}

void Customer::takeOrder() {
    if (state == CustomerState::READY_TO_ORDER) {
        setState(CustomerState::ORDER_TAKEN);
    }
}

void Customer::receiveFood() {
    if (state == CustomerState::ORDER_TAKEN) {
        setState(CustomerState::FOOD_DELIVERED);
        // Reset eating timer
        stateTimer = 0.f;
    }
}

void Customer::payBill() {
    if (state == CustomerState::READY_TO_PAY) {
        // Calculate tip based on patience
        float patienceRatio = patienceTimer / maxPatience;
        tipAmount = static_cast<int>(billAmount * 0.2f * patienceRatio);  // Up to 20% tip
        
        setState(CustomerState::PAID);
        this->showIndicator = false;
    }
}

void Customer::clearDishes() {
    hasDirtyDishes = false;
    if (occupiesTable) {
        occupiesTable->clearTable();
    }
}