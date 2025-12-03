#ifndef GRID_H
#define GRID_H
#include "Table.hpp"
#include <climits>
#include <algorithm>
#include <cmath>
#include "CustomerQueue.hpp"
enum EntityType {
    NONE,
    PLAYER,
    CUSTOMER,
    CHEF
};

enum TileType{
    EMPTY,
    TABLE,
    SOLID
};

struct Node {
    int x, y;
    TileType tileType;
    bool table;
    int gCost, hCost;
    Node* parent;
    
    int fCost() const { return gCost + hCost; }
};

class Grid {
private:
    int width;
    int height;
    int size;
    Player* player;

    CustomerQueue* customerQueue;
    std::vector<Customer*> seatedCustomers;

    std::vector<Character*> characters;
    std::vector<std::vector<Node>> nodes;  // 2D grid of nodes
    std::vector<Table*> tables;
    // Pathfinding helper methods
    std::vector<Node*> getNeighbours(Node* node);
    int manhattanDistance(Node* a, Node* b);

    //Selection
    EntityType selectedEntity;
    Customer* selectedCustomer;

    // Drag state
    bool isDragging;
    Customer* draggedCustomer;
    sf::Vector2f dragOffset;

    //Mouse
    bool mousePressed;
    

    void handleMousePressed(sf::Vector2f mousePos);
    void handleMouseDrag(sf::Vector2f mousePos);
    void handleMouseReleased(sf::Vector2f mousePos);
public:
    Grid(sf::RenderWindow* window, int size);
    ~Grid();

    void update(const float& dt);
    void render(sf::RenderTarget* window);
    void updateInputs(sf::Vector2i mousePos);
    // Pathfinding methods
    std::vector<sf::Vector2f> findPath(sf::Vector2f start, sf::Vector2f goal);
    void setWalkable(int gridX, int gridY, bool walkable);
    sf::Vector2f gridToPixel(int gx, int gy);
    sf::Vector2i pixelToGrid(float px, float py);
    
    void setPlayer(Player* player);
    Player* getPlayer() const;
    void addCharacter(Character* character);
    void addTable(Table* table);

    void initializeTables();

};

#endif