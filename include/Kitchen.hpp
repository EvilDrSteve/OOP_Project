#ifndef KITCHEN_HPP
#define KITCHEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include "Order.hpp"

class Kitchen {
private:
    sf::Vector2f position;          // Grid position
    sf::Vector2f size;              // Size in grid units
    int gridSize;
    
    sf::RectangleShape shape;
    sf::Texture counterTexture;
    sf::Sprite counterSprite;
    
    std::vector<Order*> pendingOrders;   // Orders being cooked
    std::vector<Order*> readyOrders;     // Orders ready for pickup
    
    
    // Visual indicators
    sf::Font font;
    sf::Text orderCountText;
    
public:
    Kitchen(sf::Vector2f gridPos, int gridSize);
    ~Kitchen();
    
    // Order management
    void addOrder(Order* order);
    Order* pickupAnyOrder();              // Pick up any ready order
    bool hasReadyOrders() const;
    
    
    void update(const float& dt);
    void render(sf::RenderTarget* window);
    
    // Collision/interaction
    sf::FloatRect getBounds() const;
    bool containsPoint(sf::Vector2f point) const;
    
    // Get tiles this occupies for pathfinding
    std::vector<sf::Vector2f> getOccupiedTiles() const;
    
    sf::Vector2f getInteractionPoint() const;  // Where player stands to interact
};

#endif