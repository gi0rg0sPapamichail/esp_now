#ifndef esp_now_HR_h
#define esp_now_HR_h

#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <vector>
#include <esp_now_HR_enums.h>

// Define constants
#define MAC_LENGTH 6         // Length of MAC address
#define STRING_LENGTH 40     // Maximum string length
#define SETTUP_ERRORS 3      // Number of setup errors

// Create a struct that contains the data of the message
typedef struct {
    char type;
    char msg_char[STRING_LENGTH];
    int msg_int;
    float msg_float;
    bool msg_bool;
} DATA;

// Function prototypes
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status); // Callback function for data sent
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len); // Callback function for data received
void Set_Data_parameters(DATA *new_struct, char type, char new_char[], int new_int, float new_float, bool new_bool); // Function to set data parameters
String getMACtoSTRING(uint8_t *mac); // Function to convert MAC address to string

// ESP_NOW_HR class definition
class ESP_NOW_HR {
  private:
    int ESP_CHANNEL; // ESP-NOW communication channel
    int ESP_COM;     // ESP-NOW communication mode
    int setup_errors[SETTUP_ERRORS]; // Array to store setup errors
    int errors = 0;  // Error counter

    bool encrypt;    // Encryption flag
    bool fail = false; // Failure flag

    uint8_t broadcastAddress[MAC_LENGTH]; // Broadcast address
    uint8_t MAC[MAC_LENGTH];              // MAC address

    esp_now_peer_info_t peerInfo; // Peer information for ESP-NOW
    
    static DATA reicived_msg; // Static variable to store received message data

  public:
    static std::vector<DATA> Vector_msg;  // Create a vector to store message data

    static int recieved_msgs_counter; // Number of received messages
    static bool RECIEVED_MSG; // Variable that indicates if a message was received

    // Constructor
    ESP_NOW_HR(int communication, int channel, uint8_t *MAC, bool encryption);

    // Destructor
    ~ESP_NOW_HR();

    // Method declarations
    void begin();               // Initialize ESP-NOW
    bool available();           // Check if a message was received
    bool FAIL_CHECK();          // Check for initialization errors
    uint8_t* getMAC();          // Get MAC address
    void setMAC(uint8_t *MAC);  // Set MAC address
    void setChannel(int channel); // Set communication channel
    static void setRecvMsg(DATA msg);
    void setWiFi_to_STA();      // Set WiFi mode to Station
    void setWiFi_to_AP();       // Set WiFi mode to Access Point
    void setWiFi_to_APSTA();    // Set WiFi mode to Access Point + Station
    void Encryption(bool encryption); // Enable/disable encryption
    void SendString(char *msg); // Send string message
    void SendString(char *msg, int channel); // Send string message on specific channel
    void SendInt(int msg);      // Send integer message
    void SendInt(int msg, int channel); // Send integer message on specific channel
    void SendFloat(float msg);  // Send float message
    void SendFloat(float msg, int channel); // Send float message on specific channel
    void sendBool(bool msg);    // Send boolean message
    void sendBool(bool msg, int channel); // Send boolean message on specific channel
    void SendFullStruct(char msg_String[], int msg_int, float msg_float, bool msg_bool); // Send full struct message
    void SendFullStruct(char msg_String[], int msg_int, float msg_float, bool msg_bool, int channel); // Send full struct message on specific channel
    DATA getLatestMsg(); // Get the latest received message
    DATA getMSG(int index); // Get message by index
};

#endif
