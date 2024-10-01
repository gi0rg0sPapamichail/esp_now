/**
 * @author George Papamichail
 */
#ifndef esp_now_HR_enums_h 
#define esp_now_HR_enums_h

///< Define constants
#define MAC_LENGTH 6         ///< Length of MAC address
#define STRING_LENGTH 40     ///< Maximum string length
#define SETTUP_ERRORS 6      ///< Number of setup errors
#define ENCRYPTION_KEY_LENGTH 16
#define VARIABLE_TYPES 17

/**
 * @brief   Enum for communication modes
 */ 
enum COMMUNICATION {
    TWO_WAY_COMMUNICATION,  ///< Bidirectional communication
    SENDER,                 ///< Sending only
    RECEIVER                ///< Receiving only
};

/**
 * @brief   Enum for initialization error types.
 */ 
enum INITIALIZATION_ERRORS {
    ESP_NOW_INITIALIZATION_ERROR,        ///< [Error] initializing ESP-NOW                              *1              
    ESP_NOW_COMMUNICATION_SETUP_ERROR,   ///< [Error] in the consructors communication parameter        *2                                         
    CHANNEL_OUT_OF_RANGRE,               ///< [Error] in the value of the channel (channel ranges 0-13)  *3                                  
    NEW_MAC_INITIALIZATION_ERROR,        ///< [Error] setting new MAC address                           *4                  
    ADD_PEER_INITIALIZATION_ERROR,       ///< [Error] adding peer                                       *5      
    MEMORY_ALLOCATION_ERROR              ///< [Error] allocating memory                                 *6  
};

/**
 * @brief   Enum for variable types.
 */
enum MSG_VARIABLE_TYPE {
    INT,        ///< integer
    SHORT,      ///< short
    LONG,       ///< long
    FLOAT,      ///< float
    DOUBLE,     ///< double
    CHAR,       ///< char
    STRING,     ///< string
    BOOL,       ///< boolean
    DATA,       ///< Data struct
    UNKNOWN     ///< Unknown type
};

#endif
