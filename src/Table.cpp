#include "Table.hpp"
#include "Customer.hpp"

Table::Table(sf::Vector2f pos, int gridSize, bool big) {
    this->occupied = false;
    this->bigTable = big;
    this->position = sf::Vector2f(pos.x * gridSize, pos.y * gridSize);
    this->gridSize = gridSize;
    this->state = TableState::EMPTY;
    this->seatedCustomer = nullptr;
    
    //Initialize based no table size
    if (big)
        this->size = sf::Vector2f(4.f * gridSize, 4.f * gridSize);
    else
        this->size = sf::Vector2f(4.f * gridSize, 3.f * gridSize);
    
    this->shape = sf::RectangleShape(this->size);
    this->shape.setFillColor(sf::Color(96, 59, 42));
    this->shape.setPosition(this->position);

    if (this->bigTable) {
        this->chairTexture.loadFromFile("assets/Table-Big-Chairs.png");
        this->tableTexture.loadFromFile("assets/Table-Big.png");
    } else {
        this->chairTexture.loadFromFile("assets/Table-Small-Chairs.png");
        this->tableTexture.loadFromFile("assets/Table-Small.png");
    }
    
    this->tableSprite.setTexture(this->tableTexture);
    this->tableSprite.setOrigin(sf::Vector2f(8.f, 11.f));
    this->tableSprite.setPosition(this->position);
    this->tableSprite.setScale(sf::Vector2f(
        this->size.x / 48, 
        this->size.y / (this->bigTable ? 48 : 32)
    ));
    
    // Setup dirty indicator
    dirtyIndicator.setRadius(8.f);
    dirtyIndicator.setFillColor(sf::Color(150, 100, 50));
    dirtyIndicator.setOutlineColor(sf::Color::Black);
    dirtyIndicator.setOutlineThickness(1.f);
}


// Return which tiles the table occupies on the grid
std::vector<sf::Vector2f> Table::getOccupiedTiles() const {
    std::vector<sf::Vector2f> tiles;
    
    for (int y = this->position.y; y < this->position.y + this->size.y; y += this->gridSize) {
        for (int x = this->position.x; x < this->position.x + this->size.x; x += this->gridSize) {
            tiles.push_back(sf::Vector2f(x / this->gridSize, y / this->gridSize));
        }
    }

    // Add chair positions
    if (this->bigTable) {
        tiles.push_back(sf::Vector2f(
            (this->position.x / this->gridSize) - 1, 
            (this->position.y / this->gridSize) + 1
        ));
        tiles.push_back(sf::Vector2f(
            (this->position.x / this->gridSize) - 1, 
            (this->position.y / this->gridSize) + 2
        ));
        tiles.push_back(sf::Vector2f(
            ((this->position.x + this->size.x) / this->gridSize), 
            (this->position.y / this->gridSize) + 1
        ));
        tiles.push_back(sf::Vector2f(
            ((this->position.x + this->size.x) / this->gridSize), 
            (this->position.y / this->gridSize) + 2
        ));
    } else {
        tiles.push_back(sf::Vector2f(
            (this->position.x / this->gridSize) - 1, 
            (this->position.y / this->gridSize) + 1
        ));
        tiles.push_back(sf::Vector2f(
            ((this->position.x + this->size.x) / this->gridSize), 
            (this->position.y / this->gridSize) + 1
        ));
    }
    
    return tiles;
}

void Table::update(const float& dt) {
    // Update dirty indicator position
    if (state == TableState::DIRTY) {
        dirtyIndicator.setPosition(
            position.x + size.x / 2 - 8,
            position.y - 20
        );
    }
}

void Table::render(sf::RenderTarget* window) {
    //Draw the chair
    this->tableSprite.setTexture(chairTexture);
    window->draw(this->tableSprite);
    
    // Draw dirty dishes indicator
    if (state == TableState::DIRTY) {
        window->draw(dirtyIndicator);
    }
}

void Table::lateRender(sf::RenderTarget* window) {
    //Draw the table
    this->tableSprite.setTexture(tableTexture);
    window->draw(this->tableSprite);
}

sf::FloatRect Table::getBounds() const {
    return this->tableSprite.getGlobalBounds();
}

int Table::getSize() const {
    return this->bigTable ? 4 : 2;
}


// Seat customers at the table
void Table::seatCustomer() {
    this->occupied = true;
    this->state = TableState::OCCUPIED;
}

void Table::seatCustomer(Customer* customer) {
    this->seatedCustomer = customer;
    this->occupied = true;
    this->state = TableState::OCCUPIED;
}

bool Table::getOccopied() const {
    return this->occupied;
}

// set table state
void Table::setState(TableState newState) {
    this->state = newState;
    
    if (newState == TableState::EMPTY) {
        this->occupied = false;
        this->seatedCustomer = nullptr;
    } else if (newState == TableState::DIRTY) {
        // Customer left but dishes remain
        this->seatedCustomer = nullptr;
    }
}


void Table::clearTable() {
    this->state = TableState::EMPTY;
    this->occupied = false;
    this->seatedCustomer = nullptr;
}

sf::Vector2f Table::getPosition() const {
    return sf::Vector2f(
        (this->tableSprite.getPosition().x / this->gridSize), 
        (this->tableSprite.getPosition().y / this->gridSize) + 2
    );
}

sf::Vector2f Table::getInteractionPoint() const {
    // Player stands in front of the table (below it)
    return sf::Vector2f(
        position.x / gridSize + size.x / (2 * gridSize),
        position.y / gridSize + size.y / gridSize
    );
}


// Getters
TableState Table::getState() const {
    return state;
}

bool Table::hasDirtyDishes() const {
    return state == TableState::DIRTY;
}

sf::Vector2f Table::getPixelPosition() const {
    return position;
}

Customer* Table::getSeatedCustomer() const{
    return this->seatedCustomer;
}
