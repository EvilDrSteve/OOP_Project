#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "State.hpp"
#include "Grid.hpp"
class GameState : public State {
    private:
        Grid grid;
    public:
    GameState(sf::RenderWindow* window, std::stack<State*>* states);
    virtual ~GameState();

    void endState();

    void update(const float& dt);
    void updateInputs(const float& dt);
    void render();
};


#endif