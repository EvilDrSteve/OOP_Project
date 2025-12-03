#include <SFML/Graphics.hpp>
#include "EndState.hpp"
#include <stack>

int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 768), "End Screen Test");

    std::stack<State*> states;

    // Create EndState with dummy values for served, lost, score
    EndState* endState = new EndState(&window, &states, 5, 2, 120);

    states.push(endState);

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update the top state
        if (!states.empty()) {
            State* currentState = states.top();
            currentState->update(dt);

            if (!currentState->getActive()) {
                delete currentState;
                states.pop();
                if (states.empty())
                    window.close();
            }
        }

        // Render
        window.clear();
        if (!states.empty())
            states.top()->render();
        window.display();
    }

    // Clean up remaining states (just in case)
    while (!states.empty()) {
        delete states.top();
        states.pop();
    }

    return 0;
}
