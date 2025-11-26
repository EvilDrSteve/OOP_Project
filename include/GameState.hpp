#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "State.hpp"
#include "Grid.hpp"
#include "Customer.hpp"
class GameState : public State {
    private:
        Grid* grid;
        int gridSize;
        Player* player;
        bool mousePressed;
        // Customer spawn system
        std::vector<Customer*> queueCustomers;
        std::vector<int> pattern = {2, 2, 4, 2, 4, 2};
        std::vector<int> spawnIntervals = {3, 8, 4, 3, 2};
        size_t nextSpawn = 0;
        float frameCounter = 0.f;

    public:
    GameState(sf::RenderWindow* window, std::stack<State*>* states);
    virtual ~GameState();

    void endState() override;

    void update(const float& dt) override;
    void updateInputs(const float& dt) override;
    void render() override;
};


#endif