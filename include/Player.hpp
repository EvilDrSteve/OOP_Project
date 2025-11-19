#ifndef PLAYER_H
#define PLAYER_H
#include "Character.hpp"

class Player : public Character {
    private:
        float movementSpeed;
    public:
        Player(int x, int y);
        ~Player();

        virtual void move(const float& dt, const float x, const float y) override;
        virtual void update(const float& dt) override;
};


#endif