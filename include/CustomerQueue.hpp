#ifndef CUSTOMER_QUEUE_H
#define CUSTOMER_QUEUE_H
#include "Customer.hpp"
#include <deque>

class CustomerQueue {
    private:
        std::deque<Customer*> waitingCustomers;
        sf::Vector2f queueStartPosition;
        float spacing;
        float spawnTimer;
        float spawnInterval;
        size_t maxQueueSize;

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
};


#endif