#ifndef CHARACTER_H
#define CHARACTER_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <stack>
#include <cstdlib>
#include <vector>

class Character {
    private:

    protected:
        sf::Vector2f position;
        sf::RectangleShape shape;
    public:
        Character() = default;
        Character(int x, int y, int width, int height);
        virtual ~Character();

        virtual void move(const float& dt, const float dirX, const float dirY) = 0;
        virtual void update(const float& dt) = 0;
        virtual void render(sf::RenderTarget* window);

        sf::Vector2f getPosition() const;
};
#endif