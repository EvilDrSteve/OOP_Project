#include "Order.hpp"

int Order::nextOrderId = 0;

Order::Order(Table* table, Customer* customer, float cookTime) {
    this->orderId = nextOrderId++;
    this->table = table;
    this->customer = customer;
    this->status = OrderStatus::PENDING;
    this->cookTime = cookTime;
    this->cookProgress = 0.f;
}

// Update order progress
void Order::update(const float& dt) {
    if (status == OrderStatus::COOKING) {
        cookProgress += dt;
        if (cookProgress >= cookTime) {
            status = OrderStatus::READY;
        }
    }
}


// Getters
int Order::getId() const {
    return orderId;
}

Table* Order::getTable() const {
    return table;
}

Customer* Order::getCustomer() const {
    return customer;
}

OrderStatus Order::getStatus() const {
    return status;
}

float Order::getCookProgress() const {
    return cookProgress / cookTime;
}

bool Order::isReady() const {
    return status == OrderStatus::READY;
}

// Setters / State Modifiers
void Order::setStatus(OrderStatus newStatus) {
    status = newStatus;
}

void Order::startCooking() {
    status = OrderStatus::COOKING;
    cookProgress = 0.f;
}

void Order::markDelivered() {
    status = OrderStatus::DELIVERED;
}
