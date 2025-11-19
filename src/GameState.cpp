#include "GameState.hpp"
#include <iostream>

GameState::GameState(sf::RenderWindow* window, std::stack<State*>* states) 
    : State(window, states), grid(window, 48), mousePressed(false) {
    std::cout << "Game State Created" << std::endl;
    
    Player* player = new Player(10, 10, 48);
    this->grid.setPlayer(player);
    
    // Obstacles
    for (int x = 5; x <= 7; x++) {
        for (int y = 3; y <= 4; y++) {
            this->grid.setWalkable(x, y, false);
        }
    }
    
    for (int x = 12; x <= 14; x++) {
        for (int y = 3; y <= 4; y++) {
            this->grid.setWalkable(x, y, false);
        }
    }
    
    for (int x = 5; x <= 7; x++) {
        for (int y = 8; y <= 9; y++) {
            this->grid.setWalkable(x, y, false);
        }
    }
    
    for (int x = 12; x <= 14; x++) {
        for (int y = 8; y <= 9; y++) {
            this->grid.setWalkable(x, y, false);
        }
    }
    
    for (int x = 8; x <= 11; x++) {
        for (int y = 13; y <= 14; y++) {
            this->grid.setWalkable(x, y, false);
        }
    }
    
    for (int x = 9; x <= 10; x++) {
        for (int y = 1; y <= 2; y++) {
            this->grid.setWalkable(x, y, false);
        }
    }
    
    for (int x = 1; x <= 1; x++) {
        for (int y = 6; y <= 8; y++) {
            this->grid.setWalkable(x, y, false);
        }
    }
}

GameState::~GameState() {
    this->endState();
}

void GameState::update(const float& dt) {
    this->checkForQuit();
    this->updateMousePos();
    this->updateInputs(dt);
    this->grid.update(dt);
}

void GameState::render() {
    this->grid.render(this->window);
}

void GameState::endState() {
    std::cout << "Game State Ended" << std::endl;
}

void GameState::updateInputs(const float& dt) {
    bool currentMouseState = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    
    //Only runs the frame mouse is pressed
    if (currentMouseState && !mousePressed) {
        
        sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
        // sf::Vector2f mousePos = this->window->mapPixelToCoords(mousePixelPos);
        
        
        Player* player = this->grid.getPlayer();
        if (player) {
            sf::Vector2f playerPos = player->getPosition();
            
            std::vector<sf::Vector2f> path = this->grid.findPath(playerPos, sf::Vector2f(mousePos.x, mousePos.y));
            
            if (!path.empty()) {
                player->setPath(path);
            }
        }
    }
    
    mousePressed = currentMouseState;
}