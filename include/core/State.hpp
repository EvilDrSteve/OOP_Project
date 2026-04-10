#ifndef STATE_H
#define STATE_H

#include "Character.hpp"

/**
 * @brief Abstract base for every screen in the game (menu, gameplay, end-of-round).
 *
 * A State holds a pointer to the shared window and to the Game's state stack
 * so it can push/pop sibling states. Concrete subclasses implement the pure
 * virtual update/render pipeline.
 *
 * Subclasses: MainMenuState, GameState, EndState.
 */
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
        virtual void lateRender() = 0;

};

#endif