#ifndef ENDSTATE_H
#define ENDSTATE_H

#include "State.hpp"
#include "Button.hpp"

class EndState : public State {
private:
    sf::RectangleShape rectangle;
    sf::Font font;

    Button* replayButton;
    Button* mainMenuButton;

    int tablesServed;
    int tablesLost;
    int levelScore;

    void initFont();
    void initButtons();

public:
    EndState(sf::RenderWindow* window, std::stack<State*>* states,
             int served, int lost, int score);
    virtual ~EndState();

    void endState() override;
    void checkForQuit() override;

    void update(const float& dt) override;
    void updateInputs(const float& dt) override;
    void render() override;
};

#endif
