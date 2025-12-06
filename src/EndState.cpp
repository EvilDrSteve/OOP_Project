#include "EndState.hpp"
#include "GameState.hpp"
#include "MainMenuState.hpp"
#include <iostream>

EndState::EndState(sf::RenderWindow* window, std::stack<State*>* states,
                   int served, int lost, int score)
    : State(window, states), tablesServed(served), tablesLost(lost), levelScore(score),
      endScreen(window, &font) 
{
    std::cout << "End State Created" << std::endl;

    initFont();
    initEndScreen();
}

EndState::~EndState() {}

void EndState::initFont() {
    if (!font.loadFromFile("assets/fonts/Emulator.ttf"))
        throw std::runtime_error("Could not load font");
}

void EndState::initEndScreen() {
    EndScreenData data;
    data.tablesServed = tablesServed;
    data.tablesLost = tablesLost;
    data.levelScore = levelScore;

    endScreen.setData(data);
}

void EndState::update(const float& dt) {
    this->checkForQuit();
    this->updateMousePos();

    sf::Vector2f mousePos = sf::Vector2f(this->mousePosWindow);
    bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    endScreen.update(mousePos, mousePressed);

    if (endScreen.replayPressed) {
        endScreen.replayPressed = false;
        this->states->push(new GameState(this->window, this->states));
    }
    if (endScreen.mainMenuPressed) {
        endScreen.mainMenuPressed = false;
        this->states->push(new MainMenuState(this->window, this->states));
    }
}

void EndState::updateInputs(const float& dt) {
}

void EndState::render() {
    endScreen.render();
}

void EndState::endState() {
    std::cout << "End State Ended" << std::endl;
}

void EndState::checkForQuit() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        this->active = false;
}

void EndState::lateRender(){
    
}