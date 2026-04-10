#include "Character.hpp"

Character::Character(int x, int y, int width, int height){
    this->shape.setSize(sf::Vector2f(width, height));
    this->position = sf::Vector2f(x, y);
    this->direction = Direction::RIGHT;
    

}
Character::Character(const std::string &textureFile)
{
    //Initialize character from texture file and set current state to idle
    this->position = sf::Vector2f(0, 0);
    
    if (!texture.loadFromFile(textureFile))
    std::cerr << "Failed to load character texture: " << textureFile << std::endl;
    {
    }
    sprite.setTexture(texture);
    float scale = 1.f;      
    sprite.setScale(scale, scale);
    baseScale = scale;     
    
    
    currentFrame = 0;
    frameTime = 0.07f; 
    frameTimer = 0.f;
    
    this->direction = Direction::RIGHT;
    this->currentAction = "idle";
    sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));


}

Character::~Character(){

}

//Add animation based on the provided frame details
void Character::addAnimation(const std::string &name, int frameCount, int frameWidth, int frameHeight, int startY)
{

    std::vector<sf::IntRect> frames;
    for (int i = 0; i < frameCount; i++)
    {
        frames.push_back(sf::IntRect(i * frameWidth, startY, frameWidth, frameHeight));
    }
    animations[name] = frames;
}

//Add animation based on the provided frame details and frame start coordinates
void Character::addAnimation(const std::string& name, int startX, int startY, int frameCount, int frameWidth, int frameHeight){
    std::vector<sf::IntRect> frames;
    for (int i = 0; i < frameCount; i++)
    {
        frames.push_back(sf::IntRect((i + startX) * frameWidth, startY * frameHeight, frameWidth, frameHeight));
    }
    animations[name] = frames;
}

//Set the current action of the character
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
     // Animate the Character
    frameTimer += dt;
    if (frameTimer >= frameTime)
    {
        frameTimer = 0.f;
        currentFrame++;
        if (currentFrame >= (int)animations[currentAction + std::to_string(this->direction)].size())
        {
            currentFrame = 0;
        }

        //Set the current frame based on the action and direction
        sprite.setTextureRect(animations[currentAction + std::to_string(this->direction)][currentFrame]);
        sf::IntRect rect = sprite.getTextureRect();

        sprite.setOrigin(rect.width / 2.f, rect.height);

    }
}