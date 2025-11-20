#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "AssetManager.hpp"
class Animation {
private:
    sf::Sprite* sprite;
    std::shared_ptr<sf::Texture> texture;
    std::vector<sf::IntRect> frames;
    float frameTime; // seconds per frame
    float timer;
    int currentIndex;
    bool playing;
public:
    Animation(sf::Sprite* s, std::shared_ptr<sf::Texture> tex, int frameWidth, int frameHeight, int frameCount, float fps);
    void update(float dt);

    void play();
    void stop();
    void reset();
};

#endif // ANIMATION_HPP
