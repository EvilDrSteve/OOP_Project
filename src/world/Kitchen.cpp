#include "Kitchen.hpp"

#include <algorithm>

#include "Table.hpp"

Kitchen::Kitchen(sf::Vector2f gridPos, int gridSize) {
    this->gridSize = gridSize;
    this->position = gridPos;
    this->size     = sf::Vector2f(13.f, 4.f);
    // Setup visual shape
    this->shape =
        sf::RectangleShape(sf::Vector2f(size.x * gridSize, size.y * gridSize));
    this->shape.setPosition(position.x * gridSize, position.y * gridSize);
    this->shape.setFillColor(
        sf::Color(120, 80, 60));  // Darker brown for counter
    this->shape.setOutlineColor(sf::Color(80, 50, 30));
    this->shape.setOutlineThickness(2.f);

    if (!this->counterTexture.loadFromFile("assets/kitchen.png")) {
        std::cout << "failed to load kitchen.png\n";
    }
    this->counterSprite.setTexture(this->counterTexture);

    float windowWidth = 1024.f;

    this->counterSprite.setPosition(windowWidth / 4, 0);
}

Kitchen::~Kitchen() {
    // Delete all the pointers
    for (Order* order : pendingOrders) {
        delete order;
    }
    pendingOrders.clear();

    for (Order* order : readyOrders) {
        delete order;
    }
    readyOrders.clear();
}

void Kitchen::addOrder(Order* order) {
    order->startCooking();
    pendingOrders.push_back(order);
}

// Pick ready orders from the kitchen
Order* Kitchen::pickupAnyOrder() {
    if (!readyOrders.empty()) {
        Order* order = readyOrders.front();
        readyOrders.erase(readyOrders.begin());
        return order;
    }
    return nullptr;
}

bool Kitchen::hasReadyOrders() const { return !readyOrders.empty(); }

// Update the kitchen (cook orders)
void Kitchen::update(const float& dt) {
    if (!pendingOrders.empty()) {
        Order* order = pendingOrders[0];

        order->update(dt);

        if (order->isReady()) {
            readyOrders.push_back(order);
            pendingOrders.erase(pendingOrders.begin());
        }
    }
}

void Kitchen::render(sf::RenderTarget* window) {
    // Draw kitchen sprite
    window->draw(this->counterSprite);

    // Draw ready order indicators
    float indicatorX = (position.x + 4) * gridSize + 5;
    float indicatorY = (position.y + 4) * gridSize - 20;

    // Display ready orders
    for (size_t i = 0; i < readyOrders.size() && i < 5; i++) {
        sf::CircleShape indicator(8.f);
        indicator.setFillColor(sf::Color::Green);
        indicator.setPosition(indicatorX + ((i + 1) * this->gridSize),
                              indicatorY);
        window->draw(indicator);
    }

    // Draw cooking progress bars
    for (size_t i = 0; i < pendingOrders.size() && i < 3; i++) {
        sf::RectangleShape progressBg(sf::Vector2f(30.f, 6.f));
        progressBg.setPosition(indicatorX + i * 35, indicatorY + 10);
        progressBg.setFillColor(sf::Color(60, 60, 60));

        sf::RectangleShape progressBar(
            sf::Vector2f(30.f * pendingOrders[i]->getCookProgress(), 6.f));
        progressBar.setPosition(indicatorX + i * 35, indicatorY + 10);
        progressBar.setFillColor(sf::Color::Yellow);

        window->draw(progressBg);
        window->draw(progressBar);
    }
}

sf::FloatRect Kitchen::getBounds() const { return shape.getGlobalBounds(); }

bool Kitchen::containsPoint(sf::Vector2f point) const {
    return getBounds().contains(point);
}

std::vector<sf::Vector2f> Kitchen::getOccupiedTiles() const {
    std::vector<sf::Vector2f> tiles;
    for (int y = 0; y < (int)size.y; y++) {
        for (int x = 0; x < (int)size.x; x++) {
            tiles.push_back(sf::Vector2f(position.x + x, position.y + y));
        }
    }
    return tiles;
}

sf::Vector2f Kitchen::getInteractionPoint() const {
    return sf::Vector2f(position.x + size.x / 2.f, position.y + size.y);
}