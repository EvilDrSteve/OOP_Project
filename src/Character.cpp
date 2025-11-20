#include "Character.hpp"

Character::Character(int x, int y, int width, int height){
    this->shape.setSize(sf::Vector2f(width, height));
    this->position = sf::Vector2f(x, y);
    this->facing = 0;
}
Character::Character(const std::string &textureFile)
{
    this->position = sf::Vector2f(0, 0);
    
    if (!texture.loadFromFile(textureFile))
    std::cerr << "Failed to load character texture: " << textureFile << std::endl;
    {
    }
    sprite.setTexture(texture);
    float scale = 1.f;      // <--- change this value to make character bigger
    sprite.setScale(scale, scale);
    baseScale = scale;      // store it (we will use it in flipping)
    
    
    currentFrame = 0;
    frameTime = 0.1f; // each frame lasts 0.1 seconds
    frameTimer = 0.f;
    
    currentAction = "idle";
    this->facing = 0;
}

Character::~Character(){
    this->facing = 0;

}


void Character::addAnimation(const std::string &name, int frameCount, int frameWidth, int frameHeight, int startY)
{
    std::vector<sf::IntRect> frames;
    for (int i = 0; i < frameCount; i++)
    {
        frames.push_back(sf::IntRect(i * frameWidth, startY, frameWidth, frameHeight));
    }
    animations[name] = frames;
}

void Character::setAction(const std::string &action)
{
    if (currentAction != action)
    {
        currentAction = action;
        currentFrame = 0;
        frameTimer = 0.f;
    }
}

void Character::render(sf::RenderTarget* window){
    window->draw(this->shape);
}

sf::Vector2f Character::getPosition() const{
    return this->sprite.getPosition();
}

void Character::animate(const float& dt){
     // Animate
    frameTimer += dt;
    if (frameTimer >= frameTime)
    {
        frameTimer = 0.f;
        currentFrame++;
        if (currentFrame >= (int)animations[currentAction].size())
        {
            currentFrame = 0;
        }
        sprite.setTextureRect(animations[currentAction][currentFrame]);
        sf::IntRect rect = sprite.getTextureRect();

        sprite.setOrigin(rect.width / 2.f, rect.height);

    }
}

