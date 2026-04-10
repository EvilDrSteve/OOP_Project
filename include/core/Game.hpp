#ifndef GAME_H
#define GAME_H

#include "GameState.hpp"
#include "MainMenuState.hpp"

/**
 * @brief Application root: owns the window, clock and state stack, and drives the main loop.
 *
 * Responsibilities:
 *  - Create and hold the SFML RenderWindow.
 *  - Maintain a stack of State pointers (MainMenuState, GameState, EndState) and
 *    forward update/render calls to the state on top.
 *  - Track delta time via an sf::Clock so states receive a dt in seconds.
 *
 * Collaborators: State (and its concrete subclasses).
 */
class Game {

    
    private:
            bool isRunning;
            double dt;
    
            sf::RenderWindow* window;
            sf::Event event;
            sf::Clock clock;
    
            std::stack<State*> states;
    
            void initStates();
    public:

        Game();
        ~Game();
        
        void init();
        void update();
        void render();
        void lateRender();
        void run();
        bool getRunning();

        void updateClock();

};

#endif