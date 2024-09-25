// #include "ESP_NOW_V1.h"
// #include "ESP_NOW_enums_V1.h"


// DATA recv_msg;

// // Constructor for ESP_NOW class
// ESP_NOW::ESP_NOW(int communication, int channel, uint8_t *MAC, bool encryption) {
//     this->ESP_CHANNEL = channel;
//     this->ESP_COM = communication;
//     for(int i=0; i<MAC_LENGTH;i++){
//         this->MAC[i] = MAC[i];
//     }
//     memcpy(this->MAC, MAC, MAC_LENGTH * sizeof(uint8_t));
//     memcpy(this->broadcastAddress, MAC, MAC_LENGTH * sizeof(uint8_t));
//     this->encrypt = encryption;
// }

// ESP_NOW::~ESP_NOW(){
//     free(this->setup_errors);
//     free(this->broadcastAddress);
// }

// // Initialization method
// void ESP_NOW::begin() {
//     // Set the WiFi module to station mode
//     WiFi.mode(WIFI_STA);

//     // Set the WiFi module to station mode
//     if (esp_now_init() != ESP_OK) {
//         this->setup_errors[this->errors] = ESP_NOW_INITIALIZATION_ERROR; 
//         this->errors++;
//     }

//     // Setup communication based on mode
//     switch(this->ESP_COM) {
//         case SENDER:
//             // Register for Send CB to get the status of Transmitted packet
//             esp_now_register_send_cb(OnDataSent);
//             break;
//         case RECIEVER:
//             esp_now_register_recv_cb(OnDataRecv);
//             break;
//         case TWO_WAY_COMMUNICATION:
//             esp_now_register_send_cb(OnDataSent);
//             esp_now_register_recv_cb(OnDataRecv);
//             break;
//     }

//     // Read the old MAC
//     Serial.print("[OLD] ESP32 Board MAC Address:  ");
//     Serial.println(WiFi.macAddress());

//       delay(100);

//     // Set new MAC
//     esp_wifi_set_mac(WIFI_IF_STA, &MAC[0]);

//       delay(100);
    
//     // Verify that the new MAC is set correctly
//     Serial.print("[NEW] ESP32 Board MAC Address:  ");
//     Serial.println(WiFi.macAddress());
//     if (!WiFi.macAddress().equals(getMACtoSTRING(getMAC()))) {
//         Serial.println("Failed to change MAC");
//         this->setup_errors[this->errors] = NEW_MAC_INITIALIZATION_ERROR;
//         this->errors++;
//     }

//     memcpy(peerInfo.peer_addr, broadcastAddress, MAC_LENGTH);
//     peerInfo.channel = this->ESP_CHANNEL;  
//     peerInfo.encrypt = this->encrypt;

//     if (esp_now_add_peer(&peerInfo) != ESP_OK) {
//         this->setup_errors[this->errors] = ADD_PEER_INITIALIZATION_ERROR;
//         this->errors++;
//     }
// }

// bool ESP_NOW::available(){
//     return RECIEVED_MSG;
// }

// // Method to check for initialization failures
// bool ESP_NOW::FAIL_CHECK() {
//     if(this->errors == 0) {
//         Serial.println("\rTHERE WERE NO INITIALIZATION ERROR");
//         return false;
//     }
//     for(int i = 0; i < this->errors; i++) {
//         if(this->setup_errors[i]==ESP_NOW_INITIALIZATION_ERROR) Serial.println("\rESP_NOW INITIALIZATION_ERROR");
//         else if(this->setup_errors[i]==NEW_MAC_INITIALIZATION_ERROR) Serial.println("\rNEW MAC INITIALIZATION ERROR");
//         else if(this->setup_errors[i]==ADD_PEER_INITIALIZATION_ERROR) Serial.println("\rADD PEER INITIALIZATION ERROR");        
//     }
//     Serial.println();
//     return true;
// }

// // Getter for MAC address
// uint8_t* ESP_NOW::getMAC() {
//     return this->MAC;
// }

// // Setter for MAC address
// void ESP_NOW::setMAC(uint8_t *MAC) {
//   memcpy(this->MAC, MAC, MAC_LENGTH);
//   memcpy(this->broadcastAddress, MAC, MAC_LENGTH);
//   memcpy(peerInfo.peer_addr, broadcastAddress, MAC_LENGTH);

//   // Dereference the pointer when passing it to esp_wifi_set_mac
//   esp_wifi_set_mac(WIFI_IF_STA, this->MAC);

//   // Print the new MAC address
//   Serial.print("[NEW] ESP32 Board MAC Address:  ");
//   Serial.println(WiFi.macAddress());
// }

// // Setter for channel
// void ESP_NOW::setChannel(int channel) {
//   this->ESP_CHANNEL = channel;
//   this->peerInfo.channel = channel;
// }

// void ESP_NOW::setRecvMsg(DATA msg) {
//     // Copy the received message data to the static variable
//     reicived_msg = msg;
// }

// // Set WiFi to AP mode
// void ESP_NOW::setWiFi_to_AP() {
//   WiFi.mode(WIFI_MODE_AP);
// }

// // Set WiFi to AP+STA mode
// void ESP_NOW::setWiFi_to_APSTA() {
//   WiFi.mode(WIFI_MODE_APSTA);
// }

// // Setter for encryption
// void ESP_NOW::Encryption(bool encryption) {
//   this->encrypt = encryption;
//   this->peerInfo.encrypt = encrypt;
// }

// // Send a string message using ESP-NOW
// void ESP_NOW::SendString(char char_msg[STRING_LENGTH]) {
//     DATA msg;

//     Set_Data_parameters(&msg,'C', char_msg, NULL, NULL, NULL);

//     bool result = esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
//     result == ESP_OK ? Serial.println("Successfully sent msg") : Serial.println("Failed to send msg");
// }

// // Send a string message on a specific channel using ESP-NOW
// void ESP_NOW::SendString(char *char_msg, int channel) {
//     int prevChannel = this->peerInfo.channel; // Store the current channel
//     this->peerInfo.channel = channel; // Set the new channel
//     DATA msg;

//     Set_Data_parameters(&msg,'C', char_msg, NULL, NULL, NULL);

//     bool result = esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
//     result == ESP_OK ? Serial.println("Successfully sent msg") : Serial.println("Failed to send msg");
//     this->peerInfo.channel = prevChannel; // Restore the previous channel
// }

// // Send an integer message using ESP-NOW
// void ESP_NOW::SendInt(int int_msg) {
//     DATA msg;

//     int_msg = (int)int_msg;

//     Set_Data_parameters(&msg,'I', NULL, int_msg, NULL, NULL);

//     bool result = esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
//     result == ESP_OK ? Serial.println("Successfully sent msg") : Serial.println("Failed to send msg");
// }

// // Send an integer message on a specific channel using ESP-NOW
// void ESP_NOW::SendInt(int int_msg, int channel) {
//     int prevChannel = this->peerInfo.channel; // Store the current channel
//     this->peerInfo.channel = channel; // Set the new channel
//     DATA msg;
//     int_msg = (int)int_msg;
//     Set_Data_parameters(&msg,'I', NULL, int_msg, NULL, NULL);
//     bool result = esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
//     result == ESP_OK ? Serial.println("Successfully sent msg") : Serial.println("Failed to send msg");
//     this->peerInfo.channel = prevChannel; // Restore the previous channel
// }

// // Send a float message using ESP-NOW
// void ESP_NOW::SendFloat(float float_msg) {
//     DATA msg;

//     float_msg = (float)float_msg;

//     Set_Data_parameters(&msg,'F', NULL, NULL, float_msg, NULL);
//     bool result = esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
//     result == ESP_OK ? Serial.println("Successfully sent msg") : Serial.println("Failed to send msg");
// }

// // Send a float message on a specific channel using ESP-NOW
// void ESP_NOW::SendFloat(float float_msg, int channel) {
//     int prevChannel = this->peerInfo.channel; // Store the current channel
//     this->peerInfo.channel = channel; // Set the new channel
//     DATA msg;

//     float_msg = (float)float_msg;

//     Set_Data_parameters(&msg,'F', NULL, NULL, float_msg, NULL);
//     bool result = esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
//     result == ESP_OK ? Serial.println("Successfully sent msg") : Serial.println("Failed to send msg");
//     this->peerInfo.channel = prevChannel; // Restore the previous channel
// }

// // Send a boolean message using ESP-NOW
// void ESP_NOW::sendBool(bool bool_msg) {
//     DATA msg;

//     bool_msg = (bool)bool_msg;

//     Set_Data_parameters(&msg,'B', NULL, NULL, NULL, bool_msg);
//     bool result = esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
//     result == ESP_OK ? Serial.println("Successfully sent msg") : Serial.println("Failed to send msg");
// }

// // Send a boolean message on a specific channel using ESP-NOW
// void ESP_NOW::sendBool(bool bool_msg, int channel) {
//     int prevChannel = this->peerInfo.channel; // Store the current channel
//     this->peerInfo.channel = channel; // Set the new channel
//     DATA msg;

//     bool_msg = (bool)bool_msg;

//     Set_Data_parameters(&msg,'B', NULL, NULL, NULL, bool_msg);
//     bool result = esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
//     result == ESP_OK ? Serial.println("Successfully sent msg") : Serial.println("Failed to send msg");
//     this->peerInfo.channel = prevChannel; // Restore the previous channel
// }

// // Send a full struct message using ESP-NOW
// void ESP_NOW::SendFullStruct(char msg_String[], int msg_int, float msg_float, bool msg_bool) {
//     DATA msg;
//     Set_Data_parameters(&msg, 'S', msg_String, msg_int, msg_float, msg_bool);
//     bool result = esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
//   result == ESP_OK ? Serial.println("Successfully sent msg") : Serial.println("Failed to send msg");
// }

// // Send a full struct message on a specific channel using ESP-NOW
// void ESP_NOW::SendFullStruct(char *msg_String, int msg_int, float msg_float, bool msg_bool, int channel){
//   DATA msg;
//   Set_Data_parameters(&msg, 'S', msg_String, msg_int, msg_float, msg_bool);
//   // Store the current channel
//   int prevChannel = this->peerInfo.channel;
//   // Set the new channel
//   this->peerInfo.channel = channel;
//   // Send the message using ESP-NOW
//   bool result = esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
//   result == ESP_OK ? Serial.println("Successfully sent msg") : Serial.println("Failed to send msg");
//   // Restore the previous channel
//   this->peerInfo.channel = prevChannel;
// }


// // Get the latest message that was sent
// DATA ESP_NOW::getLatestMsg() {
//     this->RECIEVED_MSG = false;
//     return reicived_msg; // Retrieve the last received message
// }

// DATA ESP_NOW::getMSG(int index){
//     return Vector_msg[index];
// }

// std::vector<DATA> ESP_NOW::Vector_msg;
// int ESP_NOW::recieved_msgs_counter = 0;  // Number of received messages
// bool ESP_NOW::RECIEVED_MSG = false; // Variable that indicates if the esp received a msg
// DATA ESP_NOW::reicived_msg;

// // Callback function for data sent
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("\r\nLast Packet Send Status:\t");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }


// // Callback function for receive data
// void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
//     // Check if the received data length matches the size of the DATA struct
//     // Create a temporary DATA struct to hold the received data
//     DATA receivedData;
//     // Copy the received data into the temporary struct
//     memcpy(&receivedData, incomingData, sizeof(DATA));

//     // Add the received data to the vector
//     ESP_NOW::Vector_msg.push_back(receivedData);

//     // Update the static variable to store the latest received message data
//     ESP_NOW::setRecvMsg(receivedData);

//     // Optionally, you can increment a counter or perform other actions here
//     ESP_NOW::recieved_msgs_counter++;
//     ESP_NOW::RECIEVED_MSG = true;

// }

// // Set the parameters of the data struct
// void Set_Data_parameters(DATA *new_struct,char type, char new_char[], int new_int, float new_float, bool new_bool) {
//   if (new_char != NULL) {
//     strncpy(new_struct->msg_char, new_char, STRING_LENGTH);
//   }
//   new_struct->type = type;
//   new_struct->msg_int = new_int;
//   new_struct->msg_float = new_float;
//   new_struct->msg_bool = new_bool;
// }

// String getMACtoSTRING(uint8_t *mac){
//   // Create a String to store the MAC address
//     String macString;

//     // Iterate through each byte of the MAC address
//     for (int i = 0; i < MAC_LENGTH; i++) {
//         // Convert the byte to a two-digit hexadecimal string
//         char hexBuffer[3];
//         sprintf(hexBuffer, "%02X", *(mac+i));

//         // Concatenate the hexadecimal string to the macString
//         macString += hexBuffer;

//         // Add a colon (:) after each byte except the last one
//         if (i < MAC_LENGTH - 1) {
//             macString += ":";
//         }
//     }
//     return macString;
// }
