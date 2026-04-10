#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "Inventory.hpp"
#include "Table.hpp"
#include <vector>
#include <functional>

class Order;
class Kitchen;
class Sink;
class Customer;

// Action that player will perform when reaching destination
enum class PlayerAction {
    NONE,
    INTERACT_TABLE,
    INTERACT_KITCHEN,
    INTERACT_SINK
};

/**
 * @brief The waiter controlled by the user. Walks a path supplied by the Grid
 *        and performs a queued PlayerAction (INTERACT_TABLE / _KITCHEN / _SINK)
 *        once it reaches the destination.
 *
 * Carries up to two items in its Inventory (orders, food, dirty dishes).
 * The Grid is responsible for pathfinding; this class only consumes the resulting
 * path and calls interactWithTable/Kitchen/Sink at the end.
 */
class Player : public Character {
private:
    std::vector<sf::Vector2f> currentPath;
    size_t currentStep;
    float movementSpeed;
    
    // Inventory system
    Inventory inventory;
    
    // Pending interaction
    PlayerAction pendingAction;
    Table* targetTable;
    Kitchen* targetKitchen;
    Sink* targetSink;
    
    // Visual indicator for held items
    sf::RectangleShape inventorySlot1;
    sf::RectangleShape inventorySlot2;

public:
    Player(int x, int y, int gridSize, std::string name);
    ~Player();

    void move(const float& dt, const float x, const float y);
    void setPath(const std::vector<sf::Vector2f>& path);
    bool isMoving() const;
    void update(const float& dt);
    void render(sf::RenderTarget* window);
    void initAnimations();
    sf::Sprite& getSprite();
    
    // Set destination and action
    void setDestination(const std::vector<sf::Vector2f>& path, PlayerAction action);
    void setTargetTable(Table* table) { targetTable = table; }
    void setTargetKitchen(Kitchen* kitchen) { targetKitchen = kitchen; }
    void setTargetSink(Sink* sink) { targetSink = sink; }
    
    // Inventory management
    Inventory& getInventory() { return inventory; }
    const Inventory& getInventory() const { return inventory; }
    bool canPickUp() const { return !inventory.isFull(); }
    bool isCarrying(ItemType type) const { return inventory.hasItemType(type); }
    
    // Interactions - these are called by Grid when player reaches destination
    void interactWithTable(Table* table);
    void interactWithKitchen(Kitchen* kitchen);
    void interactWithSink(Sink* sink);
    
    // Callbacks for when player finishes moving
    std::function<void()> onReachDestination;
    
    // Deprecated - kept for compatibility
    Table* selectedTable;
    
    // Render inventory UI
    void renderInventoryUI(sf::RenderTarget* window);
};

#endif