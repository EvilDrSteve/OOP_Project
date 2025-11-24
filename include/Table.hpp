#ifndef TABLE_H
#define TABLE_H
#include "Player.hpp"

class Table {
    private:
        bool occupied;
        sf::RectangleShape shape;
        int gridSize;
    public:
        sf::Vector2f position;
        sf::Vector2f size;

        Table(sf::Vector2f pos, int gridSize, bool big);

        void update(const float& dt);
        void render(sf::RenderTarget* window);

        std::vector<sf::Vector2f> getOccupiedTiles() const;
    };

#endif