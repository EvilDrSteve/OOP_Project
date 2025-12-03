#include "EndState.hpp"
#include "MainMenuState.hpp"
#include "GameState.hpp"
#include <iostream>

EndState::EndState(sf::RenderWindow* window, std::stack<State*>* states,
                   int served, int lost, int score)
    : State(window, states), tablesServed(served), tablesLost(lost), levelScore(score) 
{
    std::cout << "End State Created" << std::endl;

    this->initFont();
    this->rectangle.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
    this->rectangle.setFillColor(sf::Color(105, 80, 74));

    this->initButtons();
}

EndState::~EndState() {
    this->endState();
    delete replayButton;
    delete mainMenuButton;
}

void EndState::initFont() {
    if (!this->font.loadFromFile("assets/fonts/pixel01.TTF"))
        throw std::runtime_error("EndState: Could not load font");
}

void EndState::initButtons() {
    this->replayButton = new Button(512-100, 350, 200, 50, &this->font, "REPLAY", sf::Color::Yellow);
    this->mainMenuButton = new Button(512-100, 420, 200, 50, &this->font, "MAIN MENU", sf::Color::Yellow);
}

void EndState::update(const float& dt) {
    this->checkForQuit();
    this->updateMousePos();
    this->updateInputs(dt);

    this->replayButton->update(sf::Vector2f(this->mousePosWindow));
    this->mainMenuButton->update(sf::Vector2f(this->mousePosWindow));

    if (this->replayButton->isPressed()) {
        this->states->push(new GameState(window, states));
    }
    if (this->mainMenuButton->isPressed()) {
        this->states->push(new MainMenuState(window, states));
    }
}

void EndState::updateInputs(const float& dt) {
    
    
}

void EndState::render() {
    this->window->draw(this->rectangle);

    sf::Text title("Let's Try Again", this->font, 40);
    title.setFillColor(sf::Color::Red);
    title.setPosition(512 - title.getGlobalBounds().width / 2, 150);
    this->window->draw(title);

    sf::Text stats;
    stats.setFont(this->font);
    stats.setCharacterSize(25);
    stats.setFillColor(sf::Color::White);
    stats.setString(
        "Tables Served: " + std::to_string(this->tablesServed) + "\n" +
        "Tables Lost: " + std::to_string(this->tablesLost) + "\n" +
        "Level Score: " + std::to_string(this->levelScore)
    );
    stats.setPosition(512 - stats.getGlobalBounds().width / 2, 230);
    this->window->draw(stats);

    this->replayButton->render(this->window);
    this->mainMenuButton->render(this->window);
}

void EndState::endState() {
    std::cout << "End State Ended" << std::endl;
}

void EndState::checkForQuit() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        this->active = false;
}
