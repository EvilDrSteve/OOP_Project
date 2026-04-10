#ifndef CHARACTER_H
#define CHARACTER_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <stack>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <string.h>
enum Direction {
    RIGHT,
    UP,
    LEFT,
    DOWN
};

/**
 * @brief Abstract base for any animated actor drawn on the grid.
 *
 * Owns a texture, sprite, animation table (action -> frame rects), facing
 * Direction, and a position. Subclasses must implement move() and update()
 * to define their own behavior; animate() advances the current action's frames.
 *
 * Subclasses: Player, Chef, CustomerCharacter.
 */
class Character {
    private:
    protected:
        sf::Vector2f position;
        sf::RectangleShape shape;

        sf::Sprite sprite;
        sf::Texture texture;

        std::map<std::string, std::vector<sf::IntRect>> animations; // action -> frames
        std::string currentAction;
        int currentFrame;
        float frameTime;    // how long each frame lasts
        float frameTimer;   // elapsed time for current frame
        float baseScale;

        Direction direction;
    public:
        Character() = default;
        Character(int x, int y, int width, int height);
        Character(const std::string &textureFile);
        virtual ~Character();

        void addAnimation(const std::string& name, int frameCount, int frameWidth, int frameHeight, int startY);
        void addAnimation(const std::string& name, int startX, int startY, int frameCount, int frameWidth, int frameHeight);
        void setAction(const std::string& action);


        virtual void move(const float& dt, const float dirX, const float dirY) = 0;
        virtual void update(const float& dt) = 0;
        virtual void render(sf::RenderTarget* window);
        virtual sf::Vector2f getPosition() const;
        void animate(const float& dt);
};
#endif