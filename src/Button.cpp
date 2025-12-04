#include "Button.hpp"
#include <iostream>
Button::Button(float x, float y, float width, float height, sf::Font* font, std::string buttonText, sf::Color color){
    this->buttonState = 0;
    this->shape.setPosition(sf::Vector2f(x, y));
    this->shape.setSize(sf::Vector2f(width, height));
    this->font = font;
    this->idleColor = sf::Color(241, 206, 131);
    this->hoverColor = sf::Color(204, 255, 0);
    this->activeColor = sf::Color(208, 178, 104);
    this->shape.setFillColor(idleColor);

    this->buttonText.setFont(*this->font);
    this->buttonText.setString(buttonText);
    this->buttonText.setFillColor(sf::Color::Black);
    this->buttonText.setCharacterSize(100);

    float padding = 10;

    float scaleX = (this->shape.getSize().x - 2 * padding) / this->buttonText.getGlobalBounds().width;
    float scaleY = (this->shape.getSize().y - 2 * padding) / this->buttonText.getGlobalBounds().height;

    float scale = std::min(scaleX, scaleY);

    int newCharSize = this->buttonText.getCharacterSize() * scale;
    if (newCharSize < 1) newCharSize = 1;

    this->buttonText.setCharacterSize(newCharSize);

    this->buttonText.setOrigin(this->buttonText.getGlobalBounds().left + this->buttonText.getGlobalBounds().width / 2.f, this->buttonText.getGlobalBounds().top + this->buttonText.getGlobalBounds().height / 2.f);

    this->buttonText.setPosition(this->shape.getGlobalBounds().left + this->shape.getGlobalBounds().width / 2.f, this->shape.getGlobalBounds().top + this->shape.getGlobalBounds().height / 2.f);

}

Button::~Button(){

}

void Button::update(const sf::Vector2f mousePos){
    this->buttonState = 0;
    if(this->shape.getGlobalBounds().contains(mousePos)){
        this->buttonState = 1;

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) this->buttonState = 2;
    }

    switch(this->buttonState){
        case 0:
        this->shape.setFillColor(this->idleColor);
        break;
        case 1:
        this->shape.setFillColor(this->hoverColor);
        break;
        case 2:
        this->shape.setFillColor(this->activeColor);
        break;
        
        default: 
        this->shape.setFillColor(this->idleColor);
    }
}

// void Button::render(sf::RenderTarget* target){
//     target->draw(this->shape);
//     target->draw(this->buttonText);
// }

void Button::render(sf::RenderTarget* target){
    if (useTexture) {
        target->draw(buttonSprite); 
    } else {
        target->draw(shape);        
        target->draw(buttonText);   
    }
}

bool Button::isPressed() const{
    return this->buttonState == 2;
}

void Button::setTexture(const std::string& path) {
    if (buttonTex.loadFromFile(path)) {
        buttonSprite.setTexture(buttonTex);
        buttonSprite.setPosition(shape.getPosition());

        // Scale the sprite to fit the original rectangle
        buttonSprite.setScale(
            shape.getSize().x / buttonTex.getSize().x,
            shape.getSize().y / buttonTex.getSize().y
        );

        useTexture = true;
    }
}