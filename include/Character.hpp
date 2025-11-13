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
        sf::RectangleShape shape;
        float movementSpeed;
    public:
        Character();
        virtual ~Character();

        virtual void move(const float& dt, const float x, const float y);
        virtual void update(const float& dt);
        virtual void render(sf::RenderTarget* window);
};
#endif