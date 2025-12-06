#ifndef TABLE_HPP
#define TABLE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.hpp"

enum class TableState {
    EMPTY,              
    OCCUPIED,           
    DIRTY               
};

class Table {
private:
    bool occupied;
    bool bigTable;
    sf::Vector2f position;      // Pixel position
    sf::Vector2f size;          // Pixel size
    int gridSize;
    sf::RectangleShape shape;
    
    sf::Texture chairTexture;
    sf::Texture tableTexture;
    sf::Sprite tableSprite;
    
    TableState state;
    Customer* seatedCustomer;
    
    // Visual indicator for dirty dishes
    sf::CircleShape dirtyIndicator;
    
public:
    Table(sf::Vector2f pos, int gridSize, bool big);
    
    std::vector<sf::Vector2f> getOccupiedTiles() const;
    
    void update(const float& dt);
    void render(sf::RenderTarget* window);
    void lateRender(sf::RenderTarget* window);
    
    sf::FloatRect getBounds() const;
    int getSize() const;  // Returns group size (2 or 4)
    
    // Occupancy
    void seatCustomer();
    void seatCustomer(Customer* customer);
    bool getOccopied() const;
    Customer* getSeatedCustomer() const;
    
    TableState getState() const;
    void setState(TableState newState);
    void clearTable();
    bool hasDirtyDishes() const;

    sf::Vector2f getPosition() const;
    sf::Vector2f getPixelPosition() const;
    sf::Vector2f getInteractionPoint() const;
};

#endif