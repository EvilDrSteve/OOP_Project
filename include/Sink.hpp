#ifndef SINK_HPP
#define SINK_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Sink {
private:
    sf::Vector2f position;          // Grid position
    sf::Vector2f size;              // Size in grid units
    int gridSize;
    
    sf::RectangleShape shape;
    sf::Sprite sprite;
    sf::Texture texture;
    int dishesWashed;               // Counter for score/stats
    
    
    float frameTimer;
    float frameTime;
    int currentFrame;
public:
    Sink(sf::Vector2f gridPos, int gridSize);
    
    // Interaction
    void dropDishes();              // Player drops dishes here
    
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