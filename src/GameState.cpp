#include "GameState.hpp"

GameState::GameState(sf::RenderWindow* window) : State(window){
    std::cout << "Game State Created" << std::endl;
}

GameState::~GameState(){
    this->endState();
}

void GameState::update(const float& dt){
    this->checkForQuit();
    this->updateInputs(dt);
    this->player.update(dt);
}

void GameState::render(){
    this->player.render(this->window);
}

void GameState::endState(){
    std::cout << "Game State Ended" << std::endl;
}

void GameState::updateInputs(const float& dt){
   if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        this->player.move(dt, -1, 0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        this->player.move(dt, 0, 1);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        this->player.move(dt, 0, -1);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        this->player.move(dt, 1, 0);
}
