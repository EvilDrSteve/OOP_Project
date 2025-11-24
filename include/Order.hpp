#ifndef ORDER_H
#define ORDER_H

#include "Table.hpp"
enum OrderStatus {
    Pending,
    Cooking,
    Completed
};

class Order {
    private:
        Table* tableNo;
        OrderStatus status;

        sf::Vector2f position;

        sf::Texture texture;
        sf::Sprite sprite;
    public:
        Order();
        ~Order();

        void update(const float& dt);
        void moveWith(sf::Vector2f target);
};
#endif