#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Table;
class Customer;

enum class ItemType {
    NONE,
    ORDER,          // A written order to deliver to kitchen
    FOOD,           // Ready food to deliver to table
    DIRTY_DISHES    // Dishes to take to sink
};

struct Item {
    ItemType type;
    Table* sourceTable;      // Which table this item is associated with
    Customer* customer;      // Which customer this belongs to
    int orderId;             // Unique order identifier
    
    Item() : type(ItemType::NONE), sourceTable(nullptr), customer(nullptr), orderId(-1) {}
    
    Item(ItemType t, Table* table, Customer* cust, int id = -1) 
        : type(t), sourceTable(table), customer(cust), orderId(id) {}
    
    bool isEmpty() const { return type == ItemType::NONE; }
    
    void clear() {
        type = ItemType::NONE;
        sourceTable = nullptr;
        customer = nullptr;
        orderId = -1;
    }
};

class Inventory {
private:
    static const int MAX_SLOTS = 2;
    Item slots[MAX_SLOTS];
    
public:
    Inventory() {
        for (int i = 0; i < MAX_SLOTS; i++) {
            slots[i] = Item();
        }
    }
    
    // Try to add an item, returns true if successful
    bool addItem(const Item& item) {
        for (int i = 0; i < MAX_SLOTS; i++) {
            if (slots[i].isEmpty()) {
                slots[i] = item;
                return true;
            }
        }
        return false; // Inventory full
    }
    
    // Check if we have a specific item type
    bool hasItemType(ItemType type) const {
        for (int i = 0; i < MAX_SLOTS; i++) {
            if (slots[i].type == type) {
                return true;
            }
        }
        return false;
    }
    
    // Get item for a specific table
    Item* getItemForTable(Table* table) {
        for (int i = 0; i < MAX_SLOTS; i++) {
            if (slots[i].sourceTable == table && slots[i].type == ItemType::FOOD) {
                return &slots[i];
            }
        }
        return nullptr;
    }
    
    // Get first item of a specific type
    Item* getItemOfType(ItemType type) {
        for (int i = 0; i < MAX_SLOTS; i++) {
            if (slots[i].type == type) {
                return &slots[i];
            }
        }
        return nullptr;
    }
    
    // Remove an item
    bool removeItem(Item* item) {
        for (int i = 0; i < MAX_SLOTS; i++) {
            if (&slots[i] == item) {
                slots[i].clear();
                return true;
            }
        }
        return false;
    }
    
    // Remove first item of type
    bool removeItemOfType(ItemType type) {
        for (int i = 0; i < MAX_SLOTS; i++) {
            if (slots[i].type == type) {
                slots[i].clear();
                return true;
            }
        }
        return false;
    }
    
    // Check if inventory is full
    bool isFull() const {
        for (int i = 0; i < MAX_SLOTS; i++) {
            if (slots[i].isEmpty()) {
                return false;
            }
        }
        return true;
    }
    
    // Check if inventory is empty
    bool isEmpty() const {
        for (int i = 0; i < MAX_SLOTS; i++) {
            if (!slots[i].isEmpty()) {
                return false;
            }
        }
        return true;
    }
    
    // Get number of items
    int getItemCount() const {
        int count = 0;
        for (int i = 0; i < MAX_SLOTS; i++) {
            if (!slots[i].isEmpty()) {
                count++;
            }
        }
        return count;
    }
    
    // Get slot by index
    Item* getSlot(int index) {
        if (index >= 0 && index < MAX_SLOTS) {
            return &slots[index];
        }
        return nullptr;
    }
    
    const Item* getSlot(int index) const {
        if (index >= 0 && index < MAX_SLOTS) {
            return &slots[index];
        }
        return nullptr;
    }
};

#endif