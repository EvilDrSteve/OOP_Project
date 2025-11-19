#include "Grid.hpp"

Grid::Grid(sf::RenderWindow* window, int size){
    this->size = size;
    this->width = window->getSize().x / size;
    this->height = window->getSize().y / size;
}

Grid::~Grid(){
    while(!characters.empty()){
        delete characters.back();
        characters.pop_back();
    }
}

void Grid::addCharacter(Character* character){
    this->characters.push_back(character);
}

void Grid::render(sf::RenderTarget* window){
    for(int j = 0; j <= this->height; j++){
        sf::VertexArray line(sf::Lines, 2); 

        line[0].position = sf::Vector2f(0, j * this->size);
        line[1].position = sf::Vector2f(this->width * this->size, j * this->size);
        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;
        window->draw(line);
    }

    for(int i = 0; i <= this->width; i++){
        sf::VertexArray line(sf::Lines, 2);
        
        line[0].position = sf::Vector2f(i * this->size, 0);
        line[1].position = sf::Vector2f(i * this->size, this->width * this->size);
        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;
        window->draw(line);
    }

    for (Character* character: this->characters){
        character->render(window);
    }
}