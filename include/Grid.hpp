#ifndef GRID_H
#define GRID_H
#include "Player.hpp"

class Grid{
    private:
        int width;
        int height;
        int size;
        std::vector<Character*> characters;

    public:
        Grid(sf::RenderWindow* window, int size);
        ~Grid();

        void render(sf::RenderTarget* window);
        void addCharacter(Character* character);
};



#endif