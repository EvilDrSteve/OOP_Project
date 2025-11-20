#include "Animation.hpp"

Animation::Animation(sf::Sprite* s, std::shared_ptr<sf::Texture> tex, int frameWidth, int frameHeight, int frameCount, float fps = 8.f)
    : sprite(s),
      texture(tex),
      frameTime(1.f / fps),
      timer(0.f),
      currentIndex(0),
      playing(true) {
        
    // assume frames are laid horizontally in a single row
    for (int i = 0; i < frameCount; ++i) {
        frames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);
    }
    if (!frames.empty()) sprite->setTexture(*texture);
    if (!frames.empty()) sprite->setTextureRect(frames[0]);
}

void Animation::update(float dt) {
    if (!playing || frames.empty()) return;
    timer += dt;
    if (timer >= frameTime) {
        timer -= frameTime;
        currentIndex = (currentIndex + 1) % frames.size();
        sprite->setTextureRect(frames[currentIndex]);
    }
}

void Animation::play() { playing = true; }
void Animation::stop() { playing = false; }
void Animation::reset() {
    currentIndex = 0;
    timer        = 0;
    if (!frames.empty()) sprite->setTextureRect(frames[0]);
}
