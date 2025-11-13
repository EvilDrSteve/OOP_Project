#include "Game.hpp"


Game::Game() {
    this->init();
    this->initStates();


}
Game::~Game() {
    std::cout << "Ending Application" << std::endl;

    delete this->window;
    while(!this->states.empty()){
        delete this->states.top();
        this->states.pop();
    }

}

void Game::init() {
    this->isRunning = true;
    
    this->window = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Diner Dash", sf::Style::Default | sf::Style::Default);
    this->window->setFramerateLimit(60);
    this->window->setVerticalSyncEnabled(true);
    
}
void Game::initStates(){
    this->states.push(new MainMenuState(this->window, &this->states));
}

void Game::run() {
    while(this->getRunning()){
        this->updateClock();
        this->update();
        this->render();
    }
}
void Game::update() {
    this->handleEvents();

    if(!this->states.empty()){
        this->states.top()->update(this->dt);
        if(!this->states.top()->getActive()){
            delete this->states.top();
            this->states.pop();
        }
        
    }else this->isRunning = false;

}

void Game::updateClock(){
    this->dt = this->clock.restart().asSeconds();
}
void Game::render() {
   
    this->window->clear();

    if(!this->states.empty()) this->states.top()->render();
    this->window->display();
}

void Game::handleEvents() {

}

bool Game::getRunning() { return this->isRunning; }
