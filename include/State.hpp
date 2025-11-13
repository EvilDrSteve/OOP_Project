#ifndef STATE_H
#define STATE_H

#include "Character.hpp"

class State{
    protected:
        std::stack<State*>* states;

        sf::RenderWindow* window;
        bool active;

        sf::Vector2i mousePosWindow;
        sf::Vector2f mousePosView;
    public:
        State(sf::RenderWindow* window, std::stack<State*>* states);
        virtual ~State();

        const bool& getActive() const;
        virtual void checkForQuit();

        virtual void endState() = 0;
        virtual void update(const float& dt) = 0;
        virtual void updateInputs(const float& dt) = 0;
        virtual void updateMousePos();
        virtual void render() = 0;

};

#endif