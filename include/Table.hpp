#ifndef TABLE_H
#define TABLE_H
#include "Player.hpp"

class Table {
    private:
        bool occupied;
        bool bigTable;
        sf::RectangleShape shape;
        sf::Sprite tableSprite;
        sf::Sprite chairSprite;
        sf::Texture tableTexture;
        sf::Texture chairTexture;
        int gridSize;
    public:
        sf::Vector2f position;
        sf::Vector2f size;

        Table(sf::Vector2f pos, int gridSize, bool big);

        void update(const float& dt);
        void render(sf::RenderTarget* window);
        void lateRender(sf::RenderTarget* window);
        void seatCustomer();

        std::vector<sf::Vector2f> getOccupiedTiles() const;
        sf::FloatRect getBounds() const;
        bool getOccopied() const;
        int getSize() const;
        sf::Vector2f getPosition() const;
    };

#endif