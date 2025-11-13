#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
class Button {
    private:
    sf::RectangleShape shape;
    sf::Font* font;
    sf::Text buttonText;

    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;

    int buttonState;

    public:
        Button(float x, float y, float width, float height, sf::Font* font, std::string buttonText, sf::Color color);
        ~Button();

        void render(sf::RenderTarget* target);
        void update(const sf::Vector2f mousePos);

        bool isPressed() const;
};

#endif