#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "Table.hpp"

class CustomerCharacter : public Character {
private:
    int gridSize;
public:
    CustomerCharacter(const std::string& textureFile, int gridSize = 32) : Character(textureFile), gridSize(gridSize) {
        this->direction = Direction::DOWN;
    }
    
    void move(const float& dt, const float dirX, const float dirY) override {
        position.x += dirX * 100.0f * dt;
        position.y += dirY * 100.0f * dt;
        sprite.setPosition(position);
        shape.setPosition(position);
    }
    

void update(const float& dt) {
    
    this->animate(dt);
    
}

    void setPosition(sf::Vector2f position){
        this->position = position;
        setSpriteScale(this->gridSize);
    }

    void render(sf::RenderTarget* window){
        window->draw(this->sprite);
    }

    void setSpriteScale(int gridSize){
        float pixelX = this->position.x * gridSize + gridSize / 2.0f;
        float pixelY = this->position.y * gridSize + gridSize / 2.0f;
        
        this->sprite.setPosition(pixelX, pixelY);
        this->sprite.setScale(sf::Vector2f(gridSize / 12.f, gridSize / 12.f));
        sf::IntRect rect = sprite.getTextureRect();

        sprite.setOrigin(rect.width / 2.f, rect.height);

    }
    sf::Sprite& getSprite() {
        return this->sprite; // Returns a reference to the sprite object
    }
    
};


class Customer{
    private:
        int groupSize;
        sf::Vector2f position;
        
        Table* occupiesTable;
        int gridSize;
        void setupCharacters();
        std::vector<CustomerCharacter*> characters;

        // Drag functionality
        bool isDragging;
        sf::Vector2f dragOffset;
        sf::Vector2f dragStartPosition;  // To return to if invalid drop
        public:
        Customer(int size, int gridSize);
        ~Customer();
        
        void update(const float& dt);
        void render(sf::RenderTarget* window);
        void updateCharacterPositions();
        
        void setPosition(sf::Vector2f pos);
        sf::Vector2f getPosition() const;

        std::vector<CustomerCharacter*> getCharacters() const;

        
        // Drag methods
        bool containsPoint(sf::Vector2f point) const;
        void startDrag(sf::Vector2f mouseWorldPos);
        void drag(sf::Vector2f mouseWorldPos);
        void stopDrag();
        void returnToStartPosition();
        
        bool getIsDragging() const;
        int getGroupSize() const;

        void sitAtTable(Table* table);
};
#endif

