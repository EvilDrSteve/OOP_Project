#ifndef CHEF_H
#define CHEF_H

#include "Character.hpp"
#include "Order.hpp"
#include <queue>
#include <vector>
#include <SFML/Graphics.hpp>

class Grid;   // forward declaration


enum ChefState {
    IdleAtCounter,
    WalkingToStove,
    CookingAtStove,
    ReturningToCounter
};
class Chef : public Character {
private:
    float movementSpeed;
    std::vector<sf::Vector2f> currentPath;
    std::vector<sf::Vector2f>::size_type currentStep = 0;
    Order* carriedOrder = nullptr;
    Grid* grid = nullptr;
    sf::Vector2f counterPos;
    sf::Vector2f stovePos;
    std::queue<Order*> orderQueue;

    ChefState state;
    float cookingTimer;
    float cookingDuration; // 4s
    Order* currentOrder;

    

public:
    Chef(int x, int y, int gridSize = 16, std::string name = "chef");
    ~Chef();

    void move(const float& dt, const float x, const float y) override;
    void update(const float& dt) override;
    void render(sf::RenderTarget* window) override;


    void initAnimations();


    void setSpritePosition(const sf::Vector2f& pos);
    sf::Vector2f getSize() const;



};

#endif