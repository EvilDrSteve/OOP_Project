
#include "MainMenuState.hpp"
#include "GameState.hpp"
#include <iostream>

MainMenuState::MainMenuState(sf::RenderWindow* window, std::stack<State*>* states)
: State(window, states)
{
    initBackground();
    initFont();
    initTitle();
    initButton();
    initTables();
}

MainMenuState::~MainMenuState() {
    delete playButton;
}


void MainMenuState::initBackground() {
    
    woodTileTex.loadFromFile("assets/floor_tile.png");
    woodTileTex.setRepeated(true);

    background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
    background.setTexture(&woodTileTex);
    background.setTextureRect(sf::IntRect(
        0, 0,
        window->getSize().x,
        window->getSize().y
    ));
}


void MainMenuState::initFont() {
    font.loadFromFile("assets/fonts/Emulator.ttf"); 
}

void MainMenuState::initTitle() {
    title.setFont(font);
    title.setString("Diner Dash");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color(255, 210, 120));
    title.setOutlineColor(sf::Color(40, 20, 10));
    title.setOutlineThickness(4);

    float windowWidth = window->getSize().x;
    float titleWidth = title.getGlobalBounds().width;
    float titleHeight = title.getGlobalBounds().height;

    float xPos = (windowWidth - titleWidth) / 2.f;
    float yPos = 100.f; 
    title.setPosition(xPos, yPos);
}


void MainMenuState::initButton() {
    float windowWidth = window->getSize().x;
    float buttonWidth = 300.f; 
    float buttonHeight = 90.f;
    float xPos = (windowWidth - buttonWidth) / 2.f;
    float yPos = title.getPosition().y + title.getGlobalBounds().height + 50.f; 

    playButton = new Button(
        xPos, yPos,    
        buttonWidth, buttonHeight, 
        &font,
        "",
        sf::Color::White
    );
    playButton->setTexture("assets/play_button.png");
}


void MainMenuState::initTables() {
    tableTex1.loadFromFile("assets/mm_table1.png");
    tableTex2.loadFromFile("assets/mm_table2.png");
    tableTex3.loadFromFile("assets/mm_table3.png");

    table1.setTexture(tableTex1);
    table2.setTexture(tableTex2);
    table3.setTexture(tableTex3);

    table1.setScale(3.f, 3.f);
    table2.setScale(3.f, 3.f);
    table3.setScale(3.f, 3.f);

    float windowWidth = window->getSize().x;
    float windowHeight = window->getSize().y;

    float yPos = windowHeight - table1.getGlobalBounds().height - 50.f;

    float totalWidth = table1.getGlobalBounds().width +
                       table2.getGlobalBounds().width +
                       table3.getGlobalBounds().width;
    float gap = (windowWidth - totalWidth) / 4.f; 

    float x1 = gap;
    float x2 = x1 + table1.getGlobalBounds().width + gap;
    float x3 = x2 + table2.getGlobalBounds().width + gap;

    table1.setPosition(x1, yPos);
    table2.setPosition(x2, yPos);
    table3.setPosition(x3, yPos);
}

void MainMenuState::update(const float& dt) {
    updateMousePos();
    updateInputs(dt);

    playButton->update(sf::Vector2f(mousePosWindow));

    if (playButton->isPressed()) {
        states->push(new GameState(window, states));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        this->active = false;
    }

}

void MainMenuState::updateInputs(const float& dt) {}

void MainMenuState::endState() {
    std::cout << "Main Menu State Ended" << std::endl;
}

void MainMenuState::lateRender() {
}


void MainMenuState::render() {
    window->draw(background);
    window->draw(title);
    

    window->draw(table1);
    window->draw(table2);
    window->draw(table3);

    playButton->render(window);
}