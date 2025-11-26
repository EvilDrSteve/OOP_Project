#include "Customer.hpp"
#include <iostream>
#include <cmath>

Customer::Customer(int size)
    : groupSize(size), inQueue(true), occupiesTable(nullptr)
{
    if (groupSize == 2)
        loadTextures("assets/group2_standing.png");
    else
        loadTextures("assets/group4_standing.png");

    sprite.setTexture(standingTexture);

    sprite.setScale(2.f, 2.f);    // FIX: make customers visible
    sprite.setPosition(-200.f, -200.f);

    queueX = 0.f;
    targetYQueue = 0.f;
}

void Customer::loadTextures(const std::string& standingFile)
{
    if (!standingTexture.loadFromFile(standingFile))
        std::cout << "Failed to load: " << standingFile << std::endl;

    sprite.setTexture(standingTexture);
}

void Customer::setQueuePosition(float x, float y)
{
    queueX = x;
    targetYQueue = y;

    // start at this Y (instant)
    sprite.setPosition(queueX, y);
    inQueue = true;
}

void Customer::update(const float& dt)
{
    if (!inQueue) return;

    float y = sprite.getPosition().y;

    if (y < targetYQueue)
    {
        y += 120.f * dt;  

        if (y > targetYQueue)
            y = targetYQueue;
        
        sprite.setPosition(queueX, y);
    }
}

void Customer::sitAtTable(Table* table)
{
    if (!table) return;
}


void Customer::render(sf::RenderTarget* window)
{
    window->draw(sprite);
}

sf::Sprite& Customer::getSprite() {
    return this->sprite;
}


Customer::~Customer() {}



