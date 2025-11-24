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

        void makeOrder();
        void deliverOrder();
};

#endif