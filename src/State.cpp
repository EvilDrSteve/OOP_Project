#include "State.hpp"

State::State(sf::RenderWindow* window, std::stack<State*>* states){
    
    //Setup up
    this->window = window;
    this->active = true;

    this->states = states;
}

State::~State(){

}

// Check if this state should close
void State::checkForQuit(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        this->active = false;
}

const bool& State::getActive() const{
    return this->active;
}

// get the new mouse position every frame
void State::updateMousePos(){
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}