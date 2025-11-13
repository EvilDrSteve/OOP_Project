#include "State.hpp"

State::State(sf::RenderWindow* window, std::stack<State*>* states){
    this->window = window;
    this->active = true;

    this->states = states;
}

State::~State(){

}

void State::checkForQuit(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        this->active = false;
}

const bool& State::getActive() const{
    return this->active;
}

void State::updateMousePos(){
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}