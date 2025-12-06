
#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "State.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>

class MainMenuState : public State {
private:

    sf::Texture woodTileTex;
    sf::RectangleShape background;

    sf::Font font;
    sf::Text title;
    sf::Text subtitle;

    Button* playButton;

    sf::Texture tableTex1;
    sf::Texture tableTex2;
    sf::Texture tableTex3;

    sf::Sprite table1;
    sf::Sprite table2;
    sf::Sprite table3;

    void initBackground();
    void initFont();
    void initTitle();
    void initButton();
    void initTables();

public:
    MainMenuState(sf::RenderWindow* window, std::stack<State*>* states);
    virtual ~MainMenuState();

    void update(const float& dt) override;
    void updateInputs(const float& dt) override;
    void render() override;
    void endState() override;
    void lateRender() override;
};

#endif