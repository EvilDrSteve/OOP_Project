#ifndef CUSTOMER_QUEUE_H
#define CUSTOMER_QUEUE_H
#include "Customer.hpp"
#include <deque>

class Grid;

/**
 * @brief Spawns Customer groups at the restaurant entrance on a fixed interval
 *        and arranges them in a line until the player drags one to a table.
 *
 * Caps the queue at maxQueueSize; customers are removed via removeCustomer()
 * once they're seated (or leave angry).
 */
class CustomerQueue {
    private:
        std::deque<Customer*> waitingCustomers;
        sf::Vector2f queueStartPosition;
        float spacing;
        float spawnTimer;
        float spawnInterval;
        size_t maxQueueSize;

        Grid* grid;
        int gridSize;

    public:     

        CustomerQueue(int gridSize, sf::Vector2f startPos, float interval);
        ~CustomerQueue();

        void update(const float& dt);
        void render(sf::RenderTarget* window);

        void spawnCustomer();

        Customer* getCustomerAtPos(sf::Vector2f pos);
        void updateQueuePositions();
        void removeCustomer(Customer* customer);
        void setGrid(Grid* grid);
};


#endif