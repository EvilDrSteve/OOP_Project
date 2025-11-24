#ifndef PLAYER_H
#define PLAYER_H
#include "Character.hpp"

class Player : public Character {
private:
    float movementSpeed;
    std::vector<sf::Vector2f> currentPath;
    std::vector<sf::Vector2f>::size_type currentStep = 0;
    
public:
    Player(int x, int y, int gridSize = 16, std::string name = "adam");
    ~Player();

    void move(const float& dt, const float x, const float y) override;
    void update(const float& dt) override;
    void render(sf::RenderTarget* window) override;


    void setPath(const std::vector<sf::Vector2f>& path);
    bool isMoving() const;

};

#endif