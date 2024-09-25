#ifndef ESP_NOW_enums_V1_h
#define ESP_NOW_enums_V1_h

// Enum for communication modes
enum COMMUNICATION {
    TWO_WAY_COMMUNICATION,  // Bidirectional communication
    SENDER,                 // Sending only
    RECIEVER                // Receiving only
};

// Enum for initialization error types
enum INITIALIZATION_ERRORS {
    ESP_NOW_INITIALIZATION_ERROR,  // Error initializing ESP-NOW
    NEW_MAC_INITIALIZATION_ERROR,  // Error setting new MAC address
    ADD_PEER_INITIALIZATION_ERROR  // Error adding peer
};

#endif
