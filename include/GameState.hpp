#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "State.hpp"
#include "Grid.hpp"




class GameState : public State {
    private:
        Grid* grid;
        int gridSize;
        Player* player;

        bool mousePressed;

        bool gameStarted;

    public:
    GameState(sf::RenderWindow* window, std::stack<State*>* states);
    virtual ~GameState();

    void endState() override;

    void update(const float& dt) override;
    void updateInputs(const float& dt) override;
    void render() override;

    void setupLevel();
};


#endif