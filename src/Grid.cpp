#include "Grid.hpp"

Grid::Grid(sf::RenderWindow* window, int size) {
    this->size = size;
    this->width = window->getSize().x / size;
    this->height = window->getSize().y / size;
    
    this->nodes.resize(this->width);

    for (int x = 0; x < this->width; x++) {
        this->nodes[x].resize(this->height);
        for (int y = 0; y < this->height; y++) {
            this->nodes[x][y].x = x;
            this->nodes[x][y].y = y;
            this->nodes[x][y].walkable = true;
            this->nodes[x][y].parent = nullptr;
            this->nodes[x][y].gCost = 0;
            this->nodes[x][y].hCost = 0;
        }
    }

    this->player = nullptr;
}


Grid::~Grid() {
    while (!this->characters.empty()) {
        delete this->characters.back();
        this->characters.pop_back();
    }

    while(!this->tables.empty()){
        delete this->tables.back();
        this->tables.pop_back();
    }

    if(this->player != nullptr) delete this->player;
}

void Grid::initializeTables(){
    this->tables.push_back(new Table(sf::Vector2f(5, 3), this->size, true));
}

void Grid::addCharacter(Character* character) {
    this->characters.push_back(character);
}

void Grid::addTable(Table* table){
    this->tables.push_back(table);
    
    for(sf::Vector2f tile : table->getOccupiedTiles()){
        this->nodes[tile.y][tile.x].walkable = false;
    }
}

void Grid::setWalkable(int gridX, int gridY, bool walkable) {
    if (gridX >= 0 && gridX < width && gridY >= 0 && gridY < height) {
        this->nodes[gridX][gridY].walkable = walkable;
    }
}

sf::Vector2f Grid::gridToPixel(int gx, int gy) {
    return sf::Vector2f(gx * this->size + this->size / 2.0f, gy * this->size + this->size / 2.0f);
}

sf::Vector2i Grid::pixelToGrid(float px, float py) {
    return sf::Vector2i(static_cast<int>(px / this->size), static_cast<int>(py / this->size));
}

std::vector<Node*> Grid::getNeighbours(Node* node) {
    std::vector<Node*> neighbours;

    if (node->x > 0) neighbours.push_back(&nodes[node->x - 1][node->y]);
    if (node->x < width - 1) neighbours.push_back(&nodes[node->x + 1][node->y]);
    if (node->y > 0) neighbours.push_back(&nodes[node->x][node->y - 1]);
    if (node->y < height - 1) neighbours.push_back(&nodes[node->x][node->y + 1]);
    
    return neighbours;
}

int Grid::manhattanDistance(Node* a, Node* b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

std::vector<sf::Vector2f> Grid::findPath(sf::Vector2f start, sf::Vector2f goal) {
    sf::Vector2i startGrid = pixelToGrid(start.x, start.y);
    sf::Vector2i goalGrid = pixelToGrid(goal.x, goal.y);
    
    // Check if the start/goal are valid
    if (startGrid.x < 0 || startGrid.x >= width || startGrid.y < 0 || startGrid.y >= height || goalGrid.x < 0 || goalGrid.x >= width || goalGrid.y < 0 || goalGrid.y >= height) {
        return {};
    }
    
    Node* startNode = &nodes[startGrid.x][startGrid.y];
    Node* goalNode = &nodes[goalGrid.x][goalGrid.y];
    
    // Reset all nodes
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            nodes[x][y].gCost = INT_MAX;
            nodes[x][y].hCost = 0;
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
        
        //remove moved the elements that are not 'current' to the front adn erase deleted the elements at the end which are not needed
        pendingNodes.erase(std::remove(pendingNodes.begin(), pendingNodes.end(), current), pendingNodes.end());
        visitedNodes.push_back(current);
        
        for (Node* neighbour : getNeighbours(current)) {
            if (!neighbour->walkable) continue;

            //find function returns end iterator if item not found
            if (std::find(visitedNodes.begin(), visitedNodes.end(), neighbour) != visitedNodes.end()) continue;
            
            int newCost = current->gCost + 1;
            if (newCost < neighbour->gCost) {
                neighbour->gCost = newCost;
                neighbour->hCost = manhattanDistance(neighbour, goalNode);
                neighbour->parent = current;
                
                if (std::find(pendingNodes.begin(), pendingNodes.end(), neighbour) == pendingNodes.end()) {
                    pendingNodes.push_back(neighbour);
                }
            }
        }
    }
    
    return {}; // Path not found
}

void Grid::update(const float& dt){
    this->player->update(dt);


    for (Character* character : this->characters) {
        character->update(dt);
    }
    for (Table* table : this->tables) {
        table->update(dt);
    }
    
}



void Grid::render(sf::RenderTarget* window) {
    for (int x = 0; x < this->width; x++) {
        for (int y = 0; y < this->height; y++) {
            sf::RectangleShape cell(sf::Vector2f(this->size - 1, this->size - 1));
            cell.setPosition(x * this->size, y * this->size);
            
            if (!nodes[x][y].walkable) {
                cell.setFillColor(sf::Color(96, 59, 42)); 
            } else {
                cell.setFillColor(sf::Color(193, 154, 107)); 
            }
            
            window->draw(cell);
        }
    }
    
    
    // // Draw grid lines
    // for (int j = 0; j <= this->height; j++) {
        //     sf::VertexArray line(sf::Lines, 2);
        //     line[0].position = sf::Vector2f(0, j * this->size);
        //     line[1].position = sf::Vector2f(this->width * this->size, j * this->size);
        //     line[0].color = sf::Color::Red;
        //     line[1].color = sf::Color::Red;
        //     window->draw(line);
        // }
        
        // for (int i = 0; i <= this->width; i++) {
            //     sf::VertexArray line(sf::Lines, 2);
            //     line[0].position = sf::Vector2f(i * this->size, 0);
            //     line[1].position = sf::Vector2f(i * this->size, this->height * this->size);
            //     line[0].color = sf::Color::Red;
            //     line[1].color = sf::Color::Red;
            //     window->draw(line);
            // }
            
            for (Character* character : this->characters) {
                character->render(window);
            }
            
            
            for (Table* table : this->tables) {
                table->render(window);
            }
            player->render(window);
}


Player* Grid::getPlayer() const{
    return this->player;
}
void Grid::setPlayer(Player* player){
    this->player = player;
}
