#include "State.hpp"

State::State(sf::RenderWindow* window){
    this->window = window;
    this->active = true;
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
