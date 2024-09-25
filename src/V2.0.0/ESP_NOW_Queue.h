/**
 * @author George Papamichail
 */
#ifndef ESP_NOW_Queue_h
#define ESP_NOW_Queue_h

#include <cstddef>
#include <type_traits>
#include <Arduino.h>

#include "ESP_NOW_utils.h"

/**
 * @class   Msg_Queue
 * @brief   A template-based message queue capable of storing any type of data, including arrays.
 */
class Msg_Queue {
    private:
        /**
         * @class   father_node
         * @brief   Base class for the nodes in the queue. Allows for polymorphism between different types of data.
         */
        class father_node {
            public:
                father_node* next;
                virtual ~father_node() = default; // Virtual destructor for proper cleanup
        };

        /**
         * @class   child_Node
         * @brief   Template class that inherits from father_node. Holds either a single value or an array of values.
         * @tparam  T Type of data that the node stores.
         */
        template <typename T>
        class child_Node : public father_node {
            public:
                bool isArray;                   ///< Flag to check if the node stores an array.
                T data_recv;                    ///< Single value of type T.
                T* data_array;                  ///< Pointer to an array of values of type T.
                MSG_VARIABLE_TYPE var_info;     ///< Information about the variable type.
                int size = -1;                  ///< Size of the array (if the node stores an array).

                /**
                 * @brief   Constructor for a single value.
                 * @param   value The value to store in the node.
                 */
                child_Node(T value);

                /**
                 * @brief   Constructor for an array.
                 *
                 * @param   value Pointer to the array to store in the node.
                 * @param   size Size of the array.
                 */
                child_Node(T* value, size_t size);

                /**
                 * @brief Destructor to handle cleanup of dynamically allocated arrays.
                 */
                ~child_Node();
        };

        father_node* front; ///< Pointer to the front of the queue.
        father_node* rear;  ///< Pointer to the rear of the queue.


    public:
        /**
         * @brief   Constructor to initialize an empty queue.
         */
        Msg_Queue(); 
        
        /**
         * @brief   Destructor to clean up the Msg_Queue.
         */
        ~Msg_Queue();               

        /**
         * @brief   Adds a single value to the queue (enqueue).
         * @tparam  T The type of the value to be added.
         * @param   value The value to be added.
         */
        template<typename T>
        void add(T value);    

        /**
         * @brief   Adds an array to the queue (enqueue).
         * @tparam  T The type of the array elements.
         * @param   value Pointer to the array to be added.
         * @param   N Size of the array.
         */
        template<typename T>
        void add(T* value, int N);

        /**
         * @brief   Removes and returns a single value from the front of the queue (dequeue).
         * @tparam  T The type of the value to be returned.
         * @return  T The dequeued value.
         */
        template<typename T>
        T pop();            

        /**
         * @brief Removes and copies an array from the front of the queue (dequeue).
         * @tparam T The type of the array elements.
         * @param output Pointer to the output array where the dequeued data will be copied.
         */   
        template<typename T>
        void popArray(T* output);          

        /**
         * @brief Checks if the queue is empty.
         * 
         * @return true if the queue is empty, false otherwise.
         */
        bool isEmpty() const;    

        /**
         * @brief Checks if the front node contains an array.
         * 
         * @return 
         *          -true : The front node is an array
         *          -false : The front node is not an array.
         */
        bool isFrontArray() const; 

        /**
         * @brief Gets the data type of the front node.
         * 
         * @return 
         *          - ITN : The recieved message is type of int
         *          - SHORT : The recieved message is type of short
         *          - LONG : The recieved message is type of long
         *          - FLOAT : The recieved message is type of float
         *          - DOUBLE : The recieved message is type of double
         *          - CHAR : The recieved message is type of char
         *          - BOOL : The recieved message is type of bool
         *          - DATA : The recieved message is type of data struct
         */
        MSG_VARIABLE_TYPE data_type() const;

};


template<typename T>
Msg_Queue::child_Node<T>::child_Node(T value) : father_node(), data_recv(value), isArray(false) {
    this->next = nullptr;  // Initialize next pointer here
    if constexpr (std::is_same_v<T, int>) var_info = INT;
    else if constexpr (std::is_same_v<T, short>) var_info = SHORT;
    else if constexpr (std::is_same_v<T, long>) var_info = LONG;
    else if constexpr (std::is_same_v<T, float>) var_info = FLOAT;
    else if constexpr (std::is_same_v<T, double>) var_info = DOUBLE;
    else if constexpr (std::is_same_v<T, char>) var_info = CHAR;
    else if constexpr (std::is_same_v<T, bool>) var_info = BOOL;
    else if constexpr (std::is_same_v<T, data>) var_info = DATA;
}


template<typename T>
Msg_Queue::child_Node<T>::child_Node(T* value, size_t size) : father_node(), size(size), isArray(true) {
    if constexpr (std::is_same_v<T, int>) var_info = INT;
    else if constexpr (std::is_same_v<T, short>) var_info = SHORT;
    else if constexpr (std::is_same_v<T, long>) var_info = LONG;
    else if constexpr (std::is_same_v<T, float>) var_info = FLOAT;
    else if constexpr (std::is_same_v<T, double>) var_info = DOUBLE;
    else if constexpr (std::is_same_v<T, char>) var_info = CHAR;
    else if constexpr (std::is_same_v<T, bool>) var_info = BOOL;
    else if constexpr (std::is_same_v<T, data>) var_info = DATA;
    this->next = nullptr;  // Initialize next pointer here
    data_array = new T[size];
    memcpy(data_array, value, size * sizeof(T));
}

// Destructor for child_Node
template<typename T>
Msg_Queue::child_Node<T>::~child_Node() {
    if (size > 0) {
        delete[] data_array; // Free dynamically allocated array memory
    }
}

// Add (enqueue) a new element to the Msg_Queue (templated for any type)
template<typename T>
void Msg_Queue::add(T value) {
    child_Node<T>* newNode = new child_Node<T>(value); // Create a new node of type T
    if (rear == nullptr) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

template<typename T>
void Msg_Queue::add(T* value, int N) {
    child_Node<T>* newNode = new child_Node<T>(value, N); // Create a new node of type T
    if (rear == nullptr) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

// Pop (dequeue) an element from the Msg_Queue and return its data_recv as an int
template<typename T>
T Msg_Queue::pop() {
    if (front == nullptr) {
        return T(); // Return default-constructed object of type T
    }

    father_node* temp = front;
    front = front->next;

    if (front == nullptr) {
        rear = nullptr;
    }

    // Cast to the correct type
    child_Node<T>* node = static_cast<child_Node<T>*>(temp);
    T value = node ? node->data_recv : T(); // Return the value if cast is successful
    delete node;

    return value;
}

// Pop (dequeue) an element from the Msg_Queue and return its data_recv as a pointer for arrays
template<typename T>
void Msg_Queue::popArray(T* output) {
    if (front == nullptr) {
        return; // Return null if the queue is empty
    }

    father_node* temp = front;
    front = front->next;

    if (front == nullptr) {
        rear = nullptr;
    }

    // Cast to the correct type
    child_Node<T>* node = static_cast<child_Node<T>*>(temp);
    T* value = node ? node->data_array : nullptr; // Return the array pointer if cast is successful
    size_t size = node->size;

    for(int i=0; i<size; i++){
        output[i] = *(value+i);
    }
    
    delete node;

    
}
#endif