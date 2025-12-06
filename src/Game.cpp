#include "Game.hpp"


Game::Game() {
    this->init();
    this->initStates();
}
Game::~Game() {

    delete this->window; //Delete the window pointer

    // Delete all the state pointers in the stack
    while(!this->states.empty()){
        delete this->states.top();
        this->states.pop();
    }

}

void Game::init() {
    // Initialize the window and running variable
    this->isRunning = true;
    
    this->window = new sf::RenderWindow(sf::VideoMode(1024, 576, 32), "Diner Dash", sf::Style::Default | sf::Style::Default);
    this->window->setFramerateLimit(60);
    this->window->setVerticalSyncEnabled(true);
    
}
void Game::initStates(){
    // Begin with only the menu state in the stack
    this->states.push(new MainMenuState(this->window, &this->states));
}

void Game::run() {
    // Main loop, calls all the other functions while the game is running
    while(this->getRunning()){
        this->updateClock();
        this->update();
        this->render();
        this->lateRender();
    }
}
void Game::update() {

    //update the top state in the stack
    if(!this->states.empty()){
        this->states.top()->update(this->dt);

        //If the state is no longer active, delete the pointer and remove it from the stack
        if(!this->states.top()->getActive()){
            delete this->states.top();
            this->states.pop();

            if(this->states.empty()) this->isRunning = false;
        }
        // If there are no states in the stack, quit the game
    }else this->isRunning = false;

}

void Game::updateClock(){
    //Update the delta time variable which is used for time based movement
    this->dt = this->clock.restart().asSeconds();
}
void Game::render() {
   
    this->window->clear();
// Render the top state in the stack
    if(!this->states.empty()) this->states.top()->render();
}

//The render functionality is split into two draw calls, two allow for multiple layers of drawing
void Game::lateRender() {
//    Call the late render on the top stack
    if(!this->states.empty()) this->states.top()->lateRender();
    this->window->display();
}

bool Game::getRunning() { return this->isRunning; }
