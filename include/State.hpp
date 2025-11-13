#ifndef STATE_H
#define STATE_H

#include "Character.hpp"

class State{
    protected:
        sf::RenderWindow* window;
        bool active;
    public:
        State(sf::RenderWindow* window);
        virtual ~State();

        const bool& getActive() const;
        virtual void checkForQuit();

        virtual void endState() = 0;
        virtual void update(const float& dt) = 0;
        virtual void updateInputs(const float& dt) = 0;
        virtual void render() = 0;

};

#endif