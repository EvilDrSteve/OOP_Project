#include "State.hpp"
#include "EndScreen.hpp"

class EndState : public State {
private:
    sf::Font font;
    EndScreen endScreen; 

    int tablesServed;
    int tablesLost;
    int levelScore;

    void initFont();
    void initEndScreen();

public:
    EndState(sf::RenderWindow* window, std::stack<State*>* states,
             int served, int lost, int score);
    virtual ~EndState();

    void endState() override;
    void checkForQuit() override;

    void update(const float& dt) override;
    void updateInputs(const float& dt) override;
    void render() override;
    void lateRender() override;
};