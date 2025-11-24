#include "GameState.hpp"

#include <iostream>

GameState::GameState(sf::RenderWindow* window, std::stack<State*>* states)
    : State(window, states), mousePressed(false) {
    std::cout << "Game State Created" << std::endl;

    this->gridSize = 32;
    this->grid   = new Grid(window, this->gridSize);
    this->player = new Player(10, 10, this->gridSize, "Adam");
    this->grid->setPlayer(this->player);
    this->grid->addCharacter(new Player(0, 7, this->gridSize, "Alex"));
    this->grid->addTable(new Table(sf::Vector2f(9, 5), this->gridSize, true));
    this->grid->addTable(new Table(sf::Vector2f(2, 3), this->gridSize, false));
    this->grid->addTable(new Table(sf::Vector2f(2, 8), this->gridSize, false));
    this->grid->addTable(new Table(sf::Vector2f(16, 3), this->gridSize, false));
    this->grid->addTable(new Table(sf::Vector2f(16, 8), this->gridSize, false));
    
}

GameState::~GameState() {
    this->endState();
    delete this->grid;
    delete this->player;

}

void GameState::update(const float& dt) {
    this->checkForQuit();
    this->updateMousePos();
    this->updateInputs(dt);
    this->grid->update(dt);
}

void GameState::render() { this->grid->render(this->window); }

void GameState::endState() { std::cout << "Game State Ended" << std::endl; }

void GameState::updateInputs(const float& dt) {
    bool currentMouseState = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    // Only runs the frame mouse is pressed
    if (currentMouseState && !mousePressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
        // sf::Vector2f mousePos =
        // this->window->mapPixelToCoords(mousePixelPos);
        this->grid->updateInputs(mousePos);
        // if (player) {
        //     sf::Vector2f playerPos = this->player->getPosition();

        //     std::vector<sf::Vector2f> path = this->grid->findPath(
        //         playerPos, sf::Vector2f(mousePos.x, mousePos.y));

        //     if (!path.empty()) {
        //         player->setPath(path);
        //     }
        // }
    }

    mousePressed = currentMouseState;
}