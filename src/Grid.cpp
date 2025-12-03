#include "Grid.hpp"

Grid::Grid(sf::RenderWindow* window, int size) {
    this->mousePressed = false;
    this->size         = size;
    this->width        = window->getSize().x / size;
    this->height       = window->getSize().y / size;
    this->isDragging = false;       
    this->draggedCustomer = nullptr;
    this->selectedCustomer = nullptr;
    this->nodes.resize(this->width);

    for (int x = 0; x < this->width; x++) {
        this->nodes[x].resize(this->height);
        for (int y = 0; y < this->height; y++) {
            this->nodes[x][y].x        = x;
            this->nodes[x][y].y        = y;
            this->nodes[x][y].tileType = TileType::EMPTY;
            this->nodes[x][y].parent   = nullptr;
            this->nodes[x][y].gCost    = 0;
            this->nodes[x][y].hCost    = 0;
        }
    }

    this->player         = nullptr;
    this->selectedEntity = EntityType::NONE;
    this->customerQueue = new CustomerQueue(this->size, sf::Vector2f(2.f, 2.f), 5.f);
}

Grid::~Grid() {

    for(Character* character : this->characters){
        delete character;
    }
    this->characters.clear();

    for(Table* table : this->tables){
        delete table;
    }
    this->tables.clear();

    for(Customer* customer : this->seatedCustomers){
        delete customer;
    }

    this->seatedCustomers.clear();

    delete this->customerQueue;
}

void Grid::initializeTables() {
    this->tables.push_back(new Table(sf::Vector2f(5, 3), this->size, true));
}

void Grid::addCharacter(Character* character) {
    this->characters.push_back(character);
}

void Grid::addTable(Table* table) {
    this->tables.push_back(table);

    for (sf::Vector2f tile : table->getOccupiedTiles()) {
        this->nodes[tile.x][tile.y].tileType = TileType::TABLE;
    }
}

void Grid::setWalkable(int gridX, int gridY, bool walkable) {
    if (gridX >= 0 && gridX < width && gridY >= 0 && gridY < height) {
        this->nodes[gridX][gridY].tileType =
            (walkable ? TileType::EMPTY : TileType::SOLID);
    }
}

sf::Vector2f Grid::gridToPixel(int gx, int gy) {
    return sf::Vector2f(gx * this->size + this->size / 2.0f,
                        gy * this->size + this->size / 2.0f);
}

sf::Vector2i Grid::pixelToGrid(float px, float py) {
    return sf::Vector2i(static_cast<int>(px / this->size),
                        static_cast<int>(py / this->size));
}

std::vector<Node*> Grid::getNeighbours(Node* node) {
    std::vector<Node*> neighbours;

    if (node->x > 0) neighbours.push_back(&nodes[node->x - 1][node->y]);
    if (node->x < width - 1) neighbours.push_back(&nodes[node->x + 1][node->y]);
    if (node->y > 0) neighbours.push_back(&nodes[node->x][node->y - 1]);
    if (node->y < height - 1)
        neighbours.push_back(&nodes[node->x][node->y + 1]);

    return neighbours;
}

int Grid::manhattanDistance(Node* a, Node* b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

std::vector<sf::Vector2f> Grid::findPath(sf::Vector2f start,
                                         sf::Vector2f goal) {
    sf::Vector2i startGrid = pixelToGrid(start.x, start.y);
    // sf::Vector2i goalGrid = pixelToGrid(goal.x, goal.y);
    sf::Vector2i goalGrid = sf::Vector2i(goal.x, goal.y);

    // Check if the start/goal are valid
    if (startGrid.x < 0 || startGrid.x >= width || startGrid.y < 0 ||
        startGrid.y >= height || goalGrid.x < 0 || goalGrid.x >= width ||
        goalGrid.y < 0 || goalGrid.y >= height) {
        return {};
    }

    Node* startNode = &nodes[startGrid.x][startGrid.y];
    Node* goalNode  = &nodes[goalGrid.x][goalGrid.y];

    // Reset all nodes
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            nodes[x][y].gCost  = INT_MAX;
            nodes[x][y].hCost  = 0;
            nodes[x][y].parent = nullptr;
        }
    }

    std::vector<Node*> pendingNodes, visitedNodes;
    pendingNodes.push_back(startNode);
    startNode->gCost = 0;
    startNode->hCost = manhattanDistance(startNode, goalNode);

    while (!pendingNodes.empty()) {
        // Find node with lowest fCost
        Node* current = pendingNodes[0];
        for (Node* n : pendingNodes) {
            if (n->fCost() < current->fCost()) {
                current = n;
            }
        }

        // Found path
        if (current == goalNode) {
            std::vector<sf::Vector2f> path;
            while (current != nullptr) {
                path.push_back(gridToPixel(current->x, current->y));
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // remove moved the elements that are not 'current' to the front adn
        // erase deleted the elements at the end which are not needed
        pendingNodes.erase(
            std::remove(pendingNodes.begin(), pendingNodes.end(), current),
            pendingNodes.end());
        visitedNodes.push_back(current);

        for (Node* neighbour : getNeighbours(current)) {
            if (neighbour->tileType != TileType::EMPTY) continue;

            // find function returns end iterator if item not found
            if (std::find(visitedNodes.begin(), visitedNodes.end(),
                          neighbour) != visitedNodes.end())
                continue;

            int newCost = current->gCost + 1;
            if (newCost < neighbour->gCost) {
                neighbour->gCost  = newCost;
                neighbour->hCost  = manhattanDistance(neighbour, goalNode);
                neighbour->parent = current;

                if (std::find(pendingNodes.begin(), pendingNodes.end(),
                              neighbour) == pendingNodes.end()) {
                    pendingNodes.push_back(neighbour);
                }
            }
        }
    }

    return {};  // Path not found
}

void Grid::handleMousePressed(sf::Vector2f mousePos) {
    if (player->getSprite().getGlobalBounds().contains(mousePos)) {
        this->selectedEntity = EntityType::PLAYER;
    } else {

        Customer* clickedCustomer = this->customerQueue->getCustomerAtPos(mousePos);
        if(clickedCustomer){
            this->selectedEntity = EntityType::CUSTOMER;
            this->selectedCustomer = clickedCustomer;
        }
    }

    if (this->selectedEntity == EntityType::CUSTOMER) {
        this->isDragging      = true;
        this->draggedCustomer = this->selectedCustomer;
        this->draggedCustomer->startDrag(mousePos);

    } else if (this->selectedEntity == EntityType::PLAYER) {

        sf::Vector2f playerPos  = this->player->getPosition();
        sf::Vector2i targetTile = pixelToGrid(mousePos.x, mousePos.y);
        while (nodes[targetTile.x][targetTile.y].tileType == TileType::TABLE) {
            targetTile.y -= 1;
        }
        std::vector<sf::Vector2f> path =
            this->findPath(playerPos, sf::Vector2f(targetTile.x, targetTile.y));

        if (!path.empty()) {
            this->player->setPath(path);
        }
    }
}

void Grid::handleMouseDrag(sf::Vector2f mousePos) {
    if (this->isDragging) {
        this->draggedCustomer->drag(mousePos);
    }
}

void Grid::handleMouseReleased(sf::Vector2f mousePos) {
    if (this->isDragging) {
        this->isDragging     = false;
        this->selectedEntity = EntityType::PLAYER;
        Table* tableToSeat   = nullptr;
        for (Table* table : this->tables) {
            if (table->getBounds().contains(mousePos)) {
                tableToSeat = table;
                break;
            }
        }

        if (tableToSeat) {
            if (tableToSeat->getSize() ==
                    this->draggedCustomer->getGroupSize() &&
                !tableToSeat->getOccopied()) {
                
                this->customerQueue->removeCustomer(this->draggedCustomer);
                this->draggedCustomer->sitAtTable(tableToSeat);
                this->seatedCustomers.push_back(this->draggedCustomer);
                this->draggedCustomer->stopDrag();
                return;
            }
        }

        this->draggedCustomer->returnToStartPosition();

        this->draggedCustomer->stopDrag();
        this->draggedCustomer = nullptr;
    }
}

void Grid::updateInputs(sf::Vector2i mousePos) {
    bool currentMouseState = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    sf::Vector2f mousePosf = sf::Vector2f(mousePos.x, mousePos.y);

    if (currentMouseState && !mousePressed) {
        this->handleMousePressed(mousePosf);
    } else if (currentMouseState && mousePressed) {
        this->handleMouseDrag(mousePosf);
    } else if (!currentMouseState && mousePressed) {
        this->handleMouseReleased(mousePosf);
    }
    mousePressed = currentMouseState;
}

void Grid::update(const float& dt) {
    this->player->update(dt);

    for (Character* character : this->characters) {
        character->update(dt);
    }

    for (Customer* customer : this->seatedCustomers) {
        customer->update(dt);
    }
    this->customerQueue->update(dt);
    for (Table* table : this->tables) {
        table->update(dt);
    }
}

void Grid::render(sf::RenderTarget* window) {
    for (int x = 0; x < this->width; x++) {
        for (int y = 0; y < this->height; y++) {
            sf::RectangleShape cell(
                sf::Vector2f(this->size - 1, this->size - 1));
            cell.setPosition(x * this->size, y * this->size);

            if (nodes[x][y].tileType != TileType::EMPTY) {
                cell.setFillColor(sf::Color(96, 59, 42));
            } else {
                cell.setFillColor(sf::Color(193, 154, 107));
            }

            window->draw(cell);
        }
    }

    for (Character* character : this->characters) {
        character->render(window);
    }

    for (Table* table : this->tables) {
        table->render(window);
    }
    for (Customer* customer : this->seatedCustomers) {
        customer->render(window);
    }
    this->customerQueue->render(window);
}

void Grid::lateRender(sf::RenderTarget* window){
    for (Table* table : this->tables) {
        table->lateRender(window);
    }
    player->render(window);

}

Player* Grid::getPlayer() const { return this->player; }
void    Grid::setPlayer(Player* player) { this->player = player; }
