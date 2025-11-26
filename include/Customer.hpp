#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Table.hpp"
#include "Character.hpp"
#include <SFML/Graphics.hpp>

class Customer : public Character {
private:
    Table* occupiesTable;

    sf::Texture standingTexture;

    int groupSize;

public:
    float queueX;
    float targetYQueue;
    bool inQueue;

    Customer(int size);

    sf::Sprite& getSprite();

    void loadTextures(const std::string& standingFile);
    void setQueuePosition(float x, float y);

    // movement
    void update(const float& dt) override;
    void move(const float& dt, const float dirX, const float dirY) override {}

    void sitAtTable(Table* table);
    void render(sf::RenderTarget* window) override;

    ~Customer();
};

#endif
