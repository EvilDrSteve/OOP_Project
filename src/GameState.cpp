#include "GameState.hpp"
#include "EndState.hpp"
#include "Chef.hpp"
#include <iostream>
GameState::GameState(sf::RenderWindow* window, std::stack<State*>* states)
    : State(window, states), mousePressed(false) {
    std::cout << "Game State Created" << std::endl;

    this->gameStarted = false;

    //Initialize the grid
    this->gridSize = 32;
    this->grid   = new Grid(window, this->gridSize);
    grid->initializeKitchen();
    grid->initializeSink();

    //Setup the level
    this->setupLevel();

    //Play music
    if (!musicPlayer.openFromFile("assets/bgMusic.mp3")) {
        std::cout << "Failed to load music\n";
    }

    musicPlayer.play();
    musicPlayer.setLoop(true);

    //Initialize the Chef
    Chef* chefAlex = new Chef(11, 3, this->gridSize, "Alex");
    this->grid->addCharacter(chefAlex);
}

GameState::~GameState() {
    //Free pointers
    this->endState();
    delete this->grid;
    delete this->player;

}

void GameState::setupLevel(){
    //Setup the tables in the level
    int startX = 10;
    int startY = 5;
    this->player = new Player(startX + 10, startY + 10, this->gridSize, "Adam");
    this->grid->setPlayer(this->player);
    this->grid->addTable(new Table(sf::Vector2f(startX + 9, startY + 5), this->gridSize, true));
    this->grid->addTable(new Table(sf::Vector2f(startX + 2, startY + 3), this->gridSize, false));
    this->grid->addTable(new Table(sf::Vector2f(startX + 2, startY + 8), this->gridSize, false));
    this->grid->addTable(new Table(sf::Vector2f(startX + 16,startY + 3), this->gridSize, false));
    this->grid->addTable(new Table(sf::Vector2f(startX + 16,startY + 8), this->gridSize, false));
    
}

void GameState::update(const float& dt) {
    //Update everything
    this->checkForQuit();
    this->updateMousePos();
    this->updateInputs(dt);
    this->grid->update(dt);
    
    
    //End the game after 5 minutes
    this->gameTimer += dt;
    if(gameTimer > 30){
        this->active = false;
        states->push(new EndState(this->window, states, 
                                  this->grid->getTablesServed(), 
                                  this->grid->getTablesLost(), 
                                  this->grid->getTotalScore()));
    }
}


//Render the grid
void GameState::render() { this->grid->render(this->window); }
void GameState::lateRender() { this->grid->lateRender(this->window); }


void GameState::endState() { }

// Update mouse position
void GameState::updateInputs(const float& dt) {

    sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
    this->grid->updateInputs(mousePos);
}