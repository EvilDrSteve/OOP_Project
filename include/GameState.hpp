#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "State.hpp"
#include "Character.hpp"

class GameState : public State {
    private:
        Character player;
    public:
    GameState(sf::RenderWindow* window);
    virtual ~GameState();

    void endState();

    void update(const float& dt);
    void updateInputs(const float& dt);
    void render();
};


#endif