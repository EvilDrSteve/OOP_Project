#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "State.hpp"
#include "Grid.hpp"
#include <SFML/Audio.hpp>
#include "Chef.hpp"


/**
 * @brief Gameplay state: owns the Grid, the background music and the level timer.
 *
 * Forwards input and dt to the Grid, and on round end constructs an EndState
 * with the final stats (tables served, tables lost, score).
 */
class GameState : public State {
    private:
        Grid* grid;
        sf::Music musicPlayer;
        int gridSize;
        Player* player;

        bool mousePressed;

        bool gameStarted;

        float gameTimer;
    public:
    GameState(sf::RenderWindow* window, std::stack<State*>* states);
    virtual ~GameState();

    void endState() override;

    void update(const float& dt) override;
    void updateInputs(const float& dt) override;
    void render() override;
    void lateRender() override;

    void setupLevel();
};


#endif