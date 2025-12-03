#include "Customer.hpp"

Customer::Customer(int size, int gridSize) {
    this->groupSize = size;
    this->position  = sf::Vector2f(4.f, 4.f);
    this->gridSize = gridSize;
    this->occupiesTable = nullptr;
    this->isDragging = false; 
    this->setupCharacters();
}

Customer::~Customer() {
    for(CustomerCharacter* character : this->characters) {
        delete character;
    }
    this->characters.clear();
}

void Customer::setupCharacters() {
    std::vector<std::string> characterFiles = {
        "assets/Modern tiles_Free/Characters_free/Adam_16x16.png",
        "assets/Modern tiles_Free/Characters_free/Alex_16x16.png",
        "assets/Modern tiles_Free/Characters_free/Amelia_16x16.png",
        "assets/Modern tiles_Free/Characters_free/Bob_16x16.png"};

    for (int i = 0; i < this->groupSize; i++) {
        int                randomIndex = rand() % characterFiles.size();
        CustomerCharacter* character =
            new CustomerCharacter(characterFiles[randomIndex], this->gridSize);

        // Set up animations for this character
        character->addAnimation("idle0", 0, 1, 6, 16, 32);
        character->addAnimation("idle1", 6, 1, 6, 16, 32);
        character->addAnimation("idle2", 12, 1, 6, 16, 32);
        character->addAnimation("idle3", 18, 1, 6, 16, 32);
        character->addAnimation("walk0", 0, 2, 6, 16, 32);
        character->addAnimation("walk1", 6, 2, 6, 16, 32);
        character->addAnimation("walk2", 12, 2, 6, 16, 32);
        character->addAnimation("walk3", 18, 2, 6, 16, 32);
        character->setAction("idle");

        character->setSpriteScale(this->gridSize);
        characters.push_back(character);

    }



    this->updateCharacterPositions();
}
void Customer::updateCharacterPositions() {
    if (groupSize == 2) {
        characters[0]->setPosition(
            sf::Vector2f(this->position.x - 0.75, this->position.y));
        characters[1]->setPosition(
            sf::Vector2f(this->position.x + 0.75, this->position.y));
    } else if (groupSize == 4) {
        characters[0]->setPosition(
            sf::Vector2f(this->position.x - 0.75, this->position.y - 0.75));
        characters[1]->setPosition(
            sf::Vector2f(this->position.x + 0.75, this->position.y - 0.75));
        characters[2]->setPosition(
            sf::Vector2f(this->position.x - 0.75, this->position.y + 0.75));
        characters[3]->setPosition(
            sf::Vector2f(this->position.x + 0.75, this->position.y + 0.75));
    }

}

void Customer::update(const float& dt){
    for(CustomerCharacter* character : this->characters){
        character->update(dt);
    }

}

void Customer::render(sf::RenderTarget* window){
    for(CustomerCharacter* character : this->characters){
        character->render(window);
    }
}

sf::Vector2f Customer::getPosition() const {
    return this->characters[0]->getPosition();
}

std::vector<CustomerCharacter*> Customer::getCharacters() const{
    return this->characters;
}

//Drag methods

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
    
    this->setPosition(sf::Vector2f(newWorldPos.x / this->gridSize, newWorldPos.y / this->gridSize));
}

void Customer::stopDrag() {
    isDragging = false;
    
    for (CustomerCharacter* character : characters) {
        sf::Sprite& sprite = character->getSprite();
        sprite.setColor(sf::Color::White);
    }
}

void Customer::setPosition(sf::Vector2f pos) {
    this->position = pos;
    updateCharacterPositions();
}

void Customer::returnToStartPosition() {
    this->setPosition(this->dragStartPosition);
}

int Customer::getGroupSize() const{
    return this->groupSize;
}

void Customer::sitAtTable(Table* table){
    this->occupiesTable = table;
    this->occupiesTable->seatCustomer();
}

bool Customer::getIsDragging() const{
    return this->isDragging;
}