#include "CustomerQueue.hpp"
#include "Grid.hpp"

CustomerQueue::CustomerQueue(int gridSize, sf::Vector2f startPos,
                             float interval) {
    this->queueStartPosition = sf::Vector2f(2.f, 2.f);
    this->spacing            = 4.f;
    this->spawnTimer         = 5.f;
    this->spawnInterval      = 5 + (rand() % 5);
    this->maxQueueSize       = 4;
    this->gridSize           = gridSize;
    this->grid               = nullptr;
}

CustomerQueue::~CustomerQueue() {
    while (!waitingCustomers.empty()) {
        delete waitingCustomers.front();
        waitingCustomers.pop_front();
    }
}

void CustomerQueue::update(const float& dt) {

    //Spawn Customer
    if (this->waitingCustomers.size() < this->maxQueueSize) {
        this->spawnTimer += dt;
        if (this->spawnTimer >= this->spawnInterval) {
            this->spawnCustomer();
            this->spawnTimer = 0.0f;
        }
    }

    //Update customers
    for (int i = 0; i < this->waitingCustomers.size(); i++) {
        Customer* customer = this->waitingCustomers[i];
        customer->update(dt);

        if (customer->leftWithoutPaying || customer->getState() == CustomerState::LEFT) {
            if (customer->leftWithoutPaying && this->grid) {
                this->grid->incrementTablesLost();
            }
            delete customer;
            this->waitingCustomers.erase(this->waitingCustomers.begin() + i);
            this->updateQueuePositions();
            i--;  
        }
    }
}

void CustomerQueue::render(sf::RenderTarget* window) {
    for (Customer* customer : this->waitingCustomers) {
        customer->render(window);
    }
}

void CustomerQueue::spawnCustomer() {
    int size = 2;
    if(rand() % 10 > 7){
        size = 4;
    }
    Customer* newCustomer = new Customer(size, this->gridSize);
    this->waitingCustomers.push_back(newCustomer);
    this->updateQueuePositions();
}

// Get the customer the mouse is currently on
Customer* CustomerQueue::getCustomerAtPos(sf::Vector2f pos) {
    for (Customer* customer : this->waitingCustomers) {
        for (CustomerCharacter* character : customer->getCharacters()) {
            sf::FloatRect bounds = character->getSprite().getGlobalBounds();
            if (bounds.contains(pos)) {
                return customer;
            }
        }
    }

    return nullptr;
}

// Update the customers in queue when a customer is removed from the queue
void CustomerQueue::updateQueuePositions() {
    int position = 0;
    for (Customer* customer : this->waitingCustomers) {
        sf::Vector2f targetPos = queueStartPosition;
        targetPos.y += position * this->spacing;

        position++;

        //Dont change the position of the customer if its being dragged
        if (customer->getIsDragging()) continue;

        customer->setPosition(targetPos);
        customer->updateCharacterPositions();
    }
}

//Remove customer from the queue
void CustomerQueue::removeCustomer(Customer* customer) {
    auto it =
        std::find(waitingCustomers.begin(), waitingCustomers.end(), customer);
    if (it != waitingCustomers.end()) {
        waitingCustomers.erase(it);
    }
    updateQueuePositions();
}

void CustomerQueue::setGrid(Grid* grid) {
    this->grid = grid;
}