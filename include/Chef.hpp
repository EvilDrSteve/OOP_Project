#ifndef CHEF_H
#define CHEF_H
#include "Character.hpp"
#include "Order.hpp"
#include "queue"

class Chef : public Character {
    private:
        std::queue<Order*> orderQueue;
    public:
        Chef();
        ~Chef();


        void update(const float& dt) override;
        void makeOrder();
        void deliverOrder();
        void addOrder(Order* order);
};

#endif