#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H
#include "State.hpp"
#include "Button.hpp"

class MainMenuState : public State {
    private:
        sf::RectangleShape rectangle;
        sf::Font font;
        Button* button;
        void initFont();
    public:
    MainMenuState(sf::RenderWindow* window, std::stack<State*>* states);
    virtual ~MainMenuState();

    void endState();
    void checkForQuit() override;

    void update(const float& dt);
    void updateInputs(const float& dt);
    void render();
};


#endif