#include "AssetManager.hpp"

std::shared_ptr<sf::Texture> AssetManager::loadTexture(const std::string& key, const std::string& filename) {
    auto tex = std::make_shared<sf::Texture>();
    if (!tex->loadFromFile(filename)) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }
    textures[key] = tex;
    return tex;
}

std::shared_ptr<sf::Font> AssetManager::loadFont(const std::string& key, const std::string& filename) {
    auto f = std::make_shared<sf::Font>();
    if (!f->loadFromFile(filename)) {
        throw std::runtime_error("Failed to load font: " + filename);
    }
    fonts[key] = f;
    return f;
}

std::shared_ptr<sf::Texture> AssetManager::getTexture(const std::string& key) {
    return textures.at(key);
}

std::shared_ptr<sf::Font> AssetManager::getFont(const std::string& key) {
    return fonts.at(key);
}