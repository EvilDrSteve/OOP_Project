#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "Table.hpp"

class Customer : public Character {
    private:
        Table* occupiesTable;

        sf::Texture standingTexture;
        sf::Texture sittingTexture;

        sf::Sprite sprite;

        int currentWaitTime;
    public:
        Customer();
        ~Customer();
};
#endif

