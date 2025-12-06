#ifndef CUSTOMER_STATE_HPP
#define CUSTOMER_STATE_HPP

enum class CustomerState {
    IN_QUEUE,           // Waiting in the queue
    BEING_SEATED,       // Being dragged to table
    SEATED,             // Just sat down, looking at menu
    READY_TO_ORDER,     // Wants to place order (show indicator)
    ORDER_TAKEN,        // Waiting for food
    FOOD_DELIVERED,     // Eating
    READY_TO_PAY,       // Finished eating, wants to pay (show indicator)
    PAID,               // Has paid, about to leave
    LEFT                // Gone, dishes remain on table
};

// Visual indicators for customer states
enum class CustomerIndicator {
    NONE,
    READY_TO_ORDER,     // Speech bubble with notepad
    WAITING_FOR_FOOD,   // Clock or hourglass
    READY_TO_PAY,       // Money/bill icon
    ANGRY,               // Customer waited too long
    EATING
};

#endif