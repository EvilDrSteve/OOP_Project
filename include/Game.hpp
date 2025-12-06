#ifndef GAME_H
#define GAME_H

#include "GameState.hpp"
#include "MainMenuState.hpp"

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