#ifndef ORDER_HPP
#define ORDER_HPP

#include <SFML/Graphics.hpp>
#include <Customer.hpp>

enum class OrderStatus {
    PENDING,        // Order taken, waiting to be dropped at kitchen
    COOKING,        // In the kitchen being prepared
    READY,          // Ready for pickup
    DELIVERED       // Delivered to customer
};

class Order {
private:
    static int nextOrderId;
    
    int orderId;
    Table* table;
    Customer* customer;
    OrderStatus status;
    float cookTime;         
    float cookProgress;     
    
public:
    Order(Table* table, Customer* customer, float cookTime = 5.0f);
    
    void update(const float& dt);
    
public:
    int getId() const;
    Table* getTable() const;
    Customer* getCustomer() const;
    OrderStatus getStatus() const;
    float getCookProgress() const;
    bool isReady() const;

    void setStatus(OrderStatus newStatus);
    void startCooking();
    void markDelivered();

};

#endif