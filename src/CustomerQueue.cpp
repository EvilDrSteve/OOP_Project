#include "CustomerQueue.hpp"

CustomerQueue::CustomerQueue(int gridSize, sf::Vector2f startPos,
                             float interval) {
    this->queueStartPosition = sf::Vector2f(2.f, 2.f);
    this->spacing            = 4.f;
    this->spawnTimer         = 0.f;
    this->spawnInterval      = interval;
    this->maxQueueSize       = 5;
    this->gridSize           = gridSize;
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
    for (Customer* customer : this->waitingCustomers) {
        customer->update(dt);
    }
}

void CustomerQueue::render(sf::RenderTarget* window) {
    for (Customer* customer : this->waitingCustomers) {
        customer->render(window);
    }
}

void CustomerQueue::spawnCustomer() {

    Customer* newCustomer = new Customer(4 - ((rand() % 2) * 2), this->gridSize);
    this->waitingCustomers.push_back(newCustomer);
    this->updateQueuePositions();
}

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
void CustomerQueue::updateQueuePositions() {
    int position = 0;
    for (Customer* customer : this->waitingCustomers) {
        sf::Vector2f targetPos = queueStartPosition;
        targetPos.y += position * this->spacing;

        position++;
        if (customer->getIsDragging()) continue;
        customer->setPosition(targetPos);
        customer->updateCharacterPositions();
    }
}

void CustomerQueue::removeCustomer(Customer* customer) {
    auto it =
        std::find(waitingCustomers.begin(), waitingCustomers.end(), customer);
    if (it != waitingCustomers.end()) {
        waitingCustomers.erase(it);
    }
    updateQueuePositions();
}