#ifndef PLAYER_H
#define PLAYER_H
#include "Character.hpp"
#include <vector>

class Player : public Character {
private:
    float movementSpeed;
    std::vector<sf::Vector2f> currentPath;
    int currentStep;
    
public:
    Player(int x, int y, int gridSize = 16);
    ~Player();

    virtual void move(const float& dt, const float x, const float y) override;
    virtual void update(const float& dt) override;
    
    void setPath(const std::vector<sf::Vector2f>& path);
    bool isMoving() const;

};

#endif