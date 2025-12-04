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
    void setDirection(Direction direction){
        this->direction = direction;
    }

};

#include "CustomerState.hpp"

class Customer {
private:
    int groupSize;
    sf::Vector2f position;
    int gridSize;
    std::vector<CustomerCharacter*> characters;
    
    // State machine
    CustomerState state;
    float stateTimer;                   // Time in current state
    float patienceTimer;                // How long before customer gets angry
    float maxPatience;                  // Maximum patience for this customer
    
    // State durations (configurable)
    float menuReadTime;                 // Time to read menu before ready to order
    float eatingTime;                   // Time to eat food
    float postEatingTime;               // Time after eating before ready to pay
    
    // Table association
    Table* occupiesTable;
    
    // Dragging state
    bool isDragging;
    sf::Vector2f dragStartPosition;
    sf::Vector2f dragOffset;
    
    // Visual indicators
    sf::Texture indicatorTexture;
    sf::Sprite indicatorSprite;
    bool showIndicator;
    CustomerIndicator currentIndicator;
    int indicatorFrames;

    sf::Texture plateTexture;
    sf::Sprite plateSprite;
    // Dishes
    bool hasDirtyDishes;
    
    // Money/tips
    int billAmount;
    int tipAmount;
    
    void setupCharacters();
    void updateIndicator();
    
    public:
    Customer(int size, int gridSize);
    ~Customer();
    void updateCharacterPositions();
    
    void update(const float& dt);
    void render(sf::RenderTarget* window);
    void lateRender(sf::RenderTarget* window);
    void renderIndicator(sf::RenderTarget* window);
    
    // Position and movement
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);
    std::vector<CustomerCharacter*> getCharacters() const;
    
    // Drag functionality
    bool containsPoint(sf::Vector2f point) const;
    void startDrag(sf::Vector2f mouseWorldPos);
    void drag(sf::Vector2f mouseWorldPos);
    void stopDrag();
    void returnToStartPosition();
    bool getIsDragging() const;
    
    // Table interaction
    int getGroupSize() const;
    void sitAtTable(Table* table);
    Table* getTable() const { return occupiesTable; }
    
    // Animation
    void setAction(const std::string& action);
    
    // State machine
    CustomerState getState() const { return state; }
    void setState(CustomerState newState);
    bool isReadyToOrder() const { return state == CustomerState::READY_TO_ORDER; }
    bool isReadyToPay() const { return state == CustomerState::READY_TO_PAY; }
    bool hasDishes() const { return hasDirtyDishes; }
    void setHasDishes(bool has) { hasDirtyDishes = has; }
    
    // Called by player interactions
    void takeOrder();           // Player takes order
    void receiveFood();         // Player delivers food
    void payBill();             // Player takes payment
    void clearDishes();         // Player picks up dishes
    
    // Money
    int getBillAmount() const { return billAmount; }
    int getTipAmount() const { return tipAmount; }
    
    // Patience/anger
    float getPatienceRatio() const { return patienceTimer / maxPatience; }
    bool isAngry() const { return patienceTimer <= 0; }
    
    // Indicator
    CustomerIndicator getCurrentIndicator() const { return currentIndicator; }
};

#endif

