#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>

class AssetManager {
private:
    std::map<std::string, std::shared_ptr<sf::Texture>> textures;
    std::map<std::string, std::shared_ptr<sf::Font>> fonts;

public:
    AssetManager() = default;
    ~AssetManager() = default;

    std::shared_ptr<sf::Texture> loadTexture(const std::string& key, const std::string& filename);

    std::shared_ptr<sf::Font> loadFont(const std::string& key, const std::string& filename);

    std::shared_ptr<sf::Texture> getTexture(const std::string& key) ;
    std::shared_ptr<sf::Font> getFont(const std::string& key);
};
#endif // ASSETMANAGER_HPP
