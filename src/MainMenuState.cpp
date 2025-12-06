
#include "MainMenuState.hpp"
#include "GameState.hpp"
#include <iostream>

MainMenuState::MainMenuState(sf::RenderWindow* window, std::stack<State*>* states)
: State(window, states)
{
    // Call the initialize functions
    initBackground();
    initFont();
    initTitle();
    initButton();
    initTables();
}

MainMenuState::~MainMenuState() {
    // Delete the button pointer
    delete playButton;
}


void MainMenuState::initBackground() {
    // Load the background

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
    // Load the fonts
    font.loadFromFile("assets/fonts/Emulator.ttf"); 
}

void MainMenuState::initTitle() {
    // Load the title
    title.setFont(font);
    title.setString("Diner Dash");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color(255, 210, 120));
    title.setOutlineColor(sf::Color(40, 20, 10));
    title.setOutlineThickness(4);

    float windowWidth = window->getSize().x;
    float titleWidth = title.getGlobalBounds().width;

    float xPos = (windowWidth - titleWidth) / 2.f;
    float yPos = 100.f; 
    title.setPosition(xPos, yPos);
}


void MainMenuState::initButton() {
    // Load the button
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
    // Load the tables used in the main menu
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

    //Get new mouse position
    updateMousePos();

    //Update the button
    playButton->update(sf::Vector2f(mousePosWindow));

    //Start the game if button is pressed
    if (playButton->isPressed()) {
        states->push(new GameState(window, states));
    }

    //Quit
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        this->active = false;
    }

}


void MainMenuState::endState() {
}

void MainMenuState::lateRender() {
}

void MainMenuState::updateInputs(const float& dt){

}

void MainMenuState::render() {
    //Render everything
    window->draw(background);
    window->draw(title);
    

    window->draw(table1);
    window->draw(table2);
    window->draw(table3);

    playButton->render(window);
}