#include "esp_now_HR_Queue.h"

// Constructor for Msg_Queue
Msg_Queue::Msg_Queue() : front(nullptr), rear(nullptr) {}

// Destructor to clean up the Msg_Queue
Msg_Queue::~Msg_Queue() {
    while (!isEmpty()) {
        father_node* temp = front;
        front = front->next;
        delete temp;  // Clean up the base node directly
    }
}

// Check if the Msg_Queue is empty
bool Msg_Queue::isEmpty() const {
    return front == nullptr;
}

// Implementation of isFrontArray
bool Msg_Queue::isFrontArray() const {
    if (front == nullptr) {
        return false; // Queue is empty, no front node
    }

    // Cast front to child_Node<T> to check isArray
    child_Node<void*>* node = static_cast<child_Node<void*>*>(front);
    return node ? node->isArray : false; // Return isArray if cast is successful
}
// Implementation of isFrontArray
 MSG_VARIABLE_TYPE Msg_Queue::data_type() const{
    if (front == nullptr) {
        return UNKNOWN; // Queue is empty, no front node
    }

    // Cast front to child_Node<T> to check isArray
    child_Node<void*>* node = static_cast<child_Node<void*>*>(front);
    return node ? node->var_info : UNKNOWN; // Return isArray if cast is successful
}

