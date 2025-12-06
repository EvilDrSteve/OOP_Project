#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <climits>
#include <algorithm>
#include "Character.hpp"
#include "Player.hpp"
#include "Table.hpp"
#include "Customer.hpp"
#include "CustomerQueue.hpp"
#include "Kitchen.hpp"
#include "Sink.hpp"
#include "Order.hpp"
#include "Inventory.hpp"

enum class EntityType {
    NONE,
    PLAYER,
    CUSTOMER,
    CHEF,
    KITCHEN,
    SINK,
    TABLE
};

enum class TileType{
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
    std::vector<std::vector<Node>> nodes;
    std::vector<Character*> characters;
    std::vector<Table*> tables;
    std::vector<Customer*> seatedCustomers;
    std::vector<Customer*> leftCustomers;  // Customers who left but dishes remain
    
    CustomerQueue* customerQueue;
    Kitchen* kitchen;
    Sink* sink;
    
    Player* player;
    EntityType selectedEntity;
    Table* selectedTable;
    Customer* selectedCustomer;
    Customer* draggedCustomer;
    
    int width;
    int height;
    int size;
    bool mousePressed;
    bool isDragging;
    bool playerWasMoving;

    // Game stats
    int totalMoney;
    int totalTips;
    int tablesServed;
    int tablesLost;

    //Floor
    sf::Texture floorTexture;
    sf::Sprite floorSprite;
    sf::Font font;

    sf::Texture kitchenTexture;
    sf::Sprite kitchenSprite;

    sf::Texture carpetTexture;
    sf::Sprite carpetSprite;
    sf::FloatRect kitchenBounds;

public:
    Grid(sf::RenderWindow* window, int size);
    ~Grid();
    
    void initializeKitchen();
    void initializeSink();
    void initFont();

    void addCharacter(Character* character);
    void addTable(Table* table);
    
    // Coordinate conversions
    sf::Vector2f gridToPixel(int gx, int gy);
    sf::Vector2i pixelToGrid(float px, float py);
    
    // Pathfinding
    std::vector<Node*> getNeighbours(Node* node);
    int manhattanDistance(Node* a, Node* b);
    std::vector<sf::Vector2f> findPath(sf::Vector2f start, sf::Vector2f goal);
    
    // Input handling
    void handleMousePressed(sf::Vector2f mousePos);
    void handleMouseDrag(sf::Vector2f mousePos);
    void handleMouseReleased(sf::Vector2f mousePos);
    void updateInputs(sf::Vector2i mousePos);
    
    // Interaction handlers
    void handleTableInteraction(Table* table);
    void handleKitchenInteraction();
    void handleSinkInteraction();
    
    // Move player to target and perform action
    void movePlayerTo(sf::Vector2f gridTarget, PlayerAction action);
    void movePlayerToTable(Table* table);
    void movePlayerToKitchen();
    void movePlayerToSink();
    
    // Game logic
    void processCustomerPayment(Customer* customer);
    void removeLeftCustomer(Customer* customer);
    
    void update(const float& dt);
    void render(sf::RenderTarget* window);
    void lateRender(sf::RenderTarget* window);
    void renderUI(sf::RenderTarget* window);
    
    // Getters/Setters
    Player* getPlayer() const;
    void setPlayer(Player* player);
    Kitchen* getKitchen() const { return kitchen; }
    Sink* getSink() const { return sink; }
    
    int getTotalMoney() const { return totalMoney; }
    int getTotalTips() const { return totalTips; }
    int getTablesServed() const;
    int getTablesLost() const;
    int getTotalScore() const;
    void incrementTablesLost();
};

#endif