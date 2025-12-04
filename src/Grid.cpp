#include "Grid.hpp"

Grid::Grid(sf::RenderWindow* window, int size) {
    this->mousePressed     = false;
    this->size             = size;
    this->width            = window->getSize().x / size;
    this->height           = window->getSize().y / size;
    this->isDragging       = false;
    this->draggedCustomer  = nullptr;
    this->selectedCustomer = nullptr;
    this->selectedTable    = nullptr;
    this->kitchen          = nullptr;
    this->sink             = nullptr;
    this->playerWasMoving  = false;
    // Initialize game stats
    this->totalMoney = 0;
    this->totalTips  = 0;

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
    this->customerQueue =
        new CustomerQueue(this->size, sf::Vector2f(2.f, 2.f), 5.f);

    this->floorTexture.loadFromFile("assets/floor.png");
    this->floorSprite.setTexture(this->floorTexture);
    this->floorSprite.setTextureRect(sf::IntRect(16, 0, 16, 16));
    this->floorSprite.setScale(this->size / 12.f, this->size / 12.f);
    
    this->initFont();
    
    this->carpetTexture.loadFromFile("assets/carpet.png");
    this->carpetSprite.setTexture(this->carpetTexture);
    this->carpetSprite.setScale(this->size / 12.f, this->size / 12.f);

}


Grid::~Grid() {
    for (Character* character : this->characters) {
        delete character;
    }
    this->characters.clear();

    for (Table* table : this->tables) {
        delete table;
    }
    this->tables.clear();

    for (Customer* customer : this->seatedCustomers) {
        delete customer;
    }
    this->seatedCustomers.clear();

    for (Customer* customer : this->leftCustomers) {
        delete customer;
    }
    this->leftCustomers.clear();

    delete this->customerQueue;

    if (this->kitchen) delete this->kitchen;
    if (this->sink) delete this->sink;
}

void Grid::initializeTables() {
    this->tables.push_back(new Table(sf::Vector2f(5, 3), this->size, true));
}

void Grid::initializeKitchen() {
    // Place kitchen at top of screen
    this->kitchen = new Kitchen(sf::Vector2f(8, 1), this->size);

    // Mark kitchen tiles as non-walkable
    for (sf::Vector2f tile : kitchen->getOccupiedTiles()) {
        if (tile.x >= 0 && tile.x < width && tile.y >= 0 && tile.y < height) {
            this->nodes[(int)tile.x][(int)tile.y].tileType = TileType::SOLID;
        }
    }
}

void Grid::initializeSink() {
    // Place sink near kitchen
    this->sink = new Sink(sf::Vector2f(22, 0), this->size);

    // Mark sink tiles as non-walkable
    for (sf::Vector2f tile : sink->getOccupiedTiles()) {
        if (tile.x >= 0 && tile.x < width && tile.y >= 0 && tile.y < height) {
            this->nodes[(int)tile.x][(int)tile.y].tileType = TileType::SOLID;
        }
    }
}

void Grid::addCharacter(Character* character) {
    this->characters.push_back(character);
}

void Grid::addTable(Table* table) {
    this->tables.push_back(table);

    for (sf::Vector2f tile : table->getOccupiedTiles()) {
        this->nodes[(int)tile.x][(int)tile.y].tileType = TileType::TABLE;
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
    sf::Vector2i goalGrid  = sf::Vector2i((int)goal.x, (int)goal.y);

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
        Node* current = pendingNodes[0];
        for (Node* n : pendingNodes) {
            if (n->fCost() < current->fCost()) {
                current = n;
            }
        }

        if (current == goalNode) {
            std::vector<sf::Vector2f> path;
            while (current != nullptr) {
                path.push_back(gridToPixel(current->x, current->y));
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        pendingNodes.erase(
            std::remove(pendingNodes.begin(), pendingNodes.end(), current),
            pendingNodes.end());
        visitedNodes.push_back(current);

        for (Node* neighbour : getNeighbours(current)) {
            if (neighbour->tileType != TileType::EMPTY) continue;

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

    return {};
}

void Grid::movePlayerTo(sf::Vector2f gridTarget, PlayerAction action) {
    sf::Vector2f playerPos = this->player->getPosition();

    // Find walkable tile near target
    sf::Vector2i targetTile((int)gridTarget.x, (int)gridTarget.y);

    // Check if target is walkable, if not find adjacent walkable tile
    if (nodes[targetTile.x][targetTile.y].tileType != TileType::EMPTY) {
        // Try tiles around the target
        std::vector<sf::Vector2i> offsets = {
            { 0,  1},
            { 0, -1},
            { 1,  0},
            {-1,  0}
        };
        for (auto& offset : offsets) {
            int nx = targetTile.x + offset.x;
            int ny = targetTile.y + offset.y;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                nodes[nx][ny].tileType == TileType::EMPTY) {
                targetTile = sf::Vector2i(nx, ny);
                break;
            }
        }
    }

    std::vector<sf::Vector2f> path =
        this->findPath(playerPos, sf::Vector2f(targetTile.x, targetTile.y));

    if (!path.empty()) {
        this->player->setDestination(path, action);
    }
}

void Grid::movePlayerToTable(Table* table) {
    sf::Vector2f interactionPoint = table->getInteractionPoint();
    player->setTargetTable(table);
    movePlayerTo(interactionPoint, PlayerAction::INTERACT_TABLE);
}

void Grid::movePlayerToKitchen() {
    if (!kitchen) return;
    sf::Vector2f interactionPoint = kitchen->getInteractionPoint();
    player->setTargetKitchen(kitchen);
    movePlayerTo(interactionPoint, PlayerAction::INTERACT_KITCHEN);
}

void Grid::movePlayerToSink() {
    if (!sink) return;
    sf::Vector2f interactionPoint = sink->getInteractionPoint();
    player->setTargetSink(sink);
    movePlayerTo(interactionPoint, PlayerAction::INTERACT_SINK);
}

void Grid::handleTableInteraction(Table* table) {
    Inventory& inv      = player->getInventory();
    Customer*  customer = table->getSeatedCustomer();

    // Priority 1: Deliver food if we have it for this table
    Item* foodItem = inv.getItemForTable(table);
    if (foodItem && foodItem->type == ItemType::FOOD) {
        if (customer && customer->getState() == CustomerState::ORDER_TAKEN) {
            customer->receiveFood();
            inv.removeItem(foodItem);
            return;
        }
    }

    // Priority 2: Take order if customer is ready
    if (customer && customer->isReadyToOrder() && !inv.isFull()) {
        customer->takeOrder();

        // Create order item
        Order* order = new Order(table, customer, 5.0f);  // 5 second cook time
        Item   orderItem(ItemType::ORDER, table, customer, order->getId());
        inv.addItem(orderItem);

       
        return;
    }

    // Priority 3: Take payment if customer is ready to pay
    if (customer && customer->isReadyToPay()) {
        processCustomerPayment(customer);
        return;
    }

    // Priority 4: Pick up dirty dishes if table is dirty
    if (table->hasDirtyDishes() && !inv.isFull()) {
        Item dishItem(ItemType::DIRTY_DISHES, table, nullptr);
        if (inv.addItem(dishItem)) {
            // Find and remove the left customer associated with this table
            for (auto it = leftCustomers.begin(); it != leftCustomers.end();
                 ++it) {
                if ((*it)->getTable() == table) {
                    (*it)->clearDishes();
                    delete *it;
                    leftCustomers.erase(it);
                    break;
                }
            }
            table->clearTable();
        }
        return;
    }
}

void Grid::handleKitchenInteraction() {
    if (!kitchen) return;

    Inventory& inv = player->getInventory();

    // Priority 1: Drop off order if we have one
    Item* orderItem = inv.getItemOfType(ItemType::ORDER);
    if (orderItem) {
        // Create order and add to kitchen
        Order* order =
            new Order(orderItem->sourceTable, orderItem->customer, 5.0f);
        kitchen->addOrder(order);
        inv.removeItem(orderItem);
        return;
    }

    // Priority 2: Pick up ready food if there is any and we have space
    if (kitchen->hasReadyOrders() && !inv.isFull()) {
        Order* readyOrder = kitchen->pickupAnyOrder();
        if (readyOrder) {
            Item foodItem(ItemType::FOOD, readyOrder->getTable(),
                          readyOrder->getCustomer(), readyOrder->getId());
            inv.addItem(foodItem);
            delete readyOrder;  // Kitchen transfers ownership
        }
        return;
    }
}

void Grid::handleSinkInteraction() {
    if (!sink) return;

    Inventory& inv = player->getInventory();

    // Drop off dishes
    if (inv.hasItemType(ItemType::DIRTY_DISHES)) {
        inv.removeItemOfType(ItemType::DIRTY_DISHES);
        sink->dropDishes();
    }
}

void Grid::processCustomerPayment(Customer* customer) {
    customer->payBill();

    // Add money
    totalMoney += customer->getBillAmount();
    totalTips += customer->getTipAmount();

    // Move customer from seated to left
    auto it =
        std::find(seatedCustomers.begin(), seatedCustomers.end(), customer);
    if (it != seatedCustomers.end()) {
        seatedCustomers.erase(it);
        leftCustomers.push_back(customer);
    }

    // Mark table as dirty
    Table* table = customer->getTable();
    if (table) {
        table->setState(TableState::DIRTY);
    }
}

void Grid::handleMousePressed(sf::Vector2f mousePos) {
    // Check if clicking on player first (for selection)
    if (player->getSprite().getGlobalBounds().contains(mousePos)) {
        this->selectedEntity = EntityType::PLAYER;
        return;
    }

    // Check if clicking on customer in queue (for dragging)
    Customer* clickedCustomer = this->customerQueue->getCustomerAtPos(mousePos);
    if (clickedCustomer) {
        this->selectedEntity   = EntityType::CUSTOMER;
        this->selectedCustomer = clickedCustomer;
        this->isDragging       = true;
        this->draggedCustomer  = this->selectedCustomer;
        this->draggedCustomer->startDrag(mousePos);
        return;
    }

    // Check if clicking on kitchen
    if (kitchen && kitchen->containsPoint(mousePos)) {
        this->selectedEntity = EntityType::KITCHEN;
        movePlayerToKitchen();
        return;
    }

    // Check if clicking on sink
    if (sink && sink->containsPoint(mousePos)) {
        this->selectedEntity = EntityType::SINK;
        movePlayerToSink();
        return;
    }

    // Check if clicking on table
    for (Table* table : this->tables) {
        if (table->getBounds().contains(mousePos)) {
            this->selectedEntity = EntityType::TABLE;
            this->selectedTable  = table;
            movePlayerToTable(table);
            return;
        }
    }

    // If nothing else, move player to clicked position
    this->selectedEntity    = EntityType::NONE;
    sf::Vector2f playerPos  = this->player->getPosition();
    sf::Vector2i targetTile = pixelToGrid(mousePos.x, mousePos.y);

    // Avoid non-walkable tiles
    while (targetTile.x >= 0 && targetTile.y >= 0 && targetTile.x < width &&
           targetTile.y < height &&
           nodes[targetTile.x][targetTile.y].tileType != TileType::EMPTY) {
        targetTile.y -= 1;
    }

    if (targetTile.y >= 0) {
        std::vector<sf::Vector2f> path =
            this->findPath(playerPos, sf::Vector2f(targetTile.x, targetTile.y));
        if (!path.empty()) {
            this->player->setPath(path);
        }
    }
}

void Grid::handleMouseDrag(sf::Vector2f mousePos) {
    if (this->isDragging && this->draggedCustomer) {
        this->draggedCustomer->drag(mousePos);
    }
}

void Grid::handleMouseReleased(sf::Vector2f mousePos) {
    if (this->isDragging && this->draggedCustomer) {
        this->isDragging     = false;
        this->selectedEntity = EntityType::PLAYER;

        Table* tableToSeat = nullptr;
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
                tableToSeat->seatCustomer(this->draggedCustomer);
                this->seatedCustomers.push_back(this->draggedCustomer);
                this->draggedCustomer->stopDrag();
                this->draggedCustomer = nullptr;
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
    sf::Vector2f mousePosf = sf::Vector2f((float)mousePos.x, (float)mousePos.y);

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
    bool playerIsMoving = player->isMoving();

    this->player->update(dt);

    // Check if player just stopped moving
    if (playerWasMoving && !player->isMoving()) {
        // Player just arrived
        if (selectedEntity == EntityType::TABLE && selectedTable) {
            handleTableInteraction(selectedTable);
            selectedTable = nullptr;
        } else if (selectedEntity == EntityType::KITCHEN) {
            handleKitchenInteraction();
        } else if (selectedEntity == EntityType::SINK) {
            handleSinkInteraction();
        }
        selectedEntity = EntityType::NONE;
    }

    playerWasMoving = playerIsMoving;

    for (Character* character : this->characters) {
        character->update(dt);
    }

    for (Customer* customer : this->seatedCustomers) {
        customer->update(dt);
    }

    // Update left customers (for dirty dish tracking)
    for (Customer* customer : this->leftCustomers) {
        customer->update(dt);
    }

    this->customerQueue->update(dt);

    for (Table* table : this->tables) {
        table->update(dt);
    }

    if (kitchen) {
        kitchen->update(dt);
    }

    if (sink) {
        sink->update(dt);
    }

    player->onReachDestination = [this]() {

    };
}

void Grid::render(sf::RenderTarget* window) {
    // Render grid tiles

    for (int x = 0; x < this->width; x++) {
        for (int y = 0; y < this->height; y++) {
            sf::RectangleShape cell(
                sf::Vector2f((float)(this->size), (float)(this->size)));
                cell.setPosition((float)(x * this->size), (float)(y * this->size));
                this->floorSprite.setPosition(
                    sf::Vector2f((float)(x * this->size), (float)(y * this->size)));
                    if (nodes[x][y].tileType != TileType::EMPTY) {
                        cell.setFillColor(sf::Color(96, 59, 42));
                    } else {
                        cell.setFillColor(sf::Color(193, 154, 107));
                    }
                    
                    cell.setFillColor(sf::Color(193, 154, 107));
                    window->draw(cell);
                    window->draw(this->floorSprite);
                }
            }
            window->draw(this->carpetSprite);

    
    for (Table* table : this->tables) {
        table->render(window);
    }
    
    for (Customer* customer : this->seatedCustomers) {
        customer->render(window);
    }
    
    if (kitchen) {
        kitchen->render(window);
    }
    for (Character* character : this->characters) {
        character->render(window);
    }

    if (sink) {
        sink->render(window);
    }

    this->customerQueue->render(window);
}

void Grid::lateRender(sf::RenderTarget* window) {
    for (Table* table : this->tables) {
        table->lateRender(window);
    }

    // Render customer indicators
    for (Customer* customer : this->seatedCustomers) {
        customer->lateRender(window);
    }
    for (Customer* customer : this->leftCustomers) {
        customer->lateRender(window);
    }

    player->render(window);

    this->renderUI(window);
}

void Grid::renderUI(sf::RenderTarget* window) {
    
    sf::Text text("$:" + std::to_string(this->totalMoney), this->font, 30);
    text.setPosition(sf::Vector2f(800, 0));
    text.setFillColor(sf::Color::White);
    window->draw(text);
}
void Grid::initFont() {
    if (!this->font.loadFromFile("assets/fonts/Emulator.ttf"))
        throw std::runtime_error("MainMenuState: Could not load font");
}
Player* Grid::getPlayer() const { return this->player; }

void Grid::setPlayer(Player* player) { this->player = player; }