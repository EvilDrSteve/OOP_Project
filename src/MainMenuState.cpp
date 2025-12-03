#include "MainMenuState.hpp"
#include "GameState.hpp"
MainMenuState::MainMenuState(sf::RenderWindow* window, std::stack<State*>* states) : State(window, states){
    std::cout << "Main Menu State Created" << std::endl;

    this->initFont();
    this->rectangle.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
    this->rectangle.setFillColor(sf::Color(105, 80, 74));

    this->button = new Button(512-100, 263, 200, 50, &this->font, "Start", sf::Color::Yellow);
}

MainMenuState::~MainMenuState(){
    this->endState();
    delete this->button;
}

void MainMenuState::update(const float& dt){
    this->checkForQuit();
    this->updateMousePos();
    this->updateInputs(dt);

    this->button->update(sf::Vector2f(this->mousePosWindow));

    if(this->button->isPressed()){
        this->states->push(new GameState(window, states));
    }
}

void MainMenuState::render(){
    this->window->draw(this->rectangle);

    sf::Text text("Main Menu", this->font, 30);

    this->window->draw(text);

    this->button->render(this->window);
}
void MainMenuState::lateRender(){
}

void MainMenuState::endState(){
    std::cout << "Main Menu State Ended" << std::endl;
}

void MainMenuState::updateInputs(const float& dt){

}


void MainMenuState::initFont(){
    if(!this->font.loadFromFile("assets/fonts/pixel01.TTF"))
        throw std::runtime_error("MainMenuState: Could not load font");
}

void MainMenuState::checkForQuit(){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        this->active = false;
}
