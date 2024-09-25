#include "Esp_Now.h"


/**************Declaration of static variables and methods**************/
uint8_t* Esp_Now::getEspMAC(){
    uint8_t *temp;
    getSTRINGtoMAC(WiFi.macAddress(), temp);
    return temp;
}

void Esp_Now::getEspMAC(uint8_t* MAC){
    getSTRINGtoMAC(WiFi.macAddress(), MAC);
}



void Esp_Now::setRecvMsg(msg_struct msg){
    if(!msg.array){
        if(msg.type == INT) Esp_Now::recieved_msgs.add<int>(msg.msg.i);
        else if(msg.type == SHORT) Esp_Now::recieved_msgs.add<short>(msg.msg.s);
        else if(msg.type == LONG) Esp_Now::recieved_msgs.add<long>(msg.msg.l);
        else if(msg.type == FLOAT) Esp_Now::recieved_msgs.add<float>(msg.msg.f);
        else if(msg.type == DOUBLE) Esp_Now::recieved_msgs.add<double>(msg.msg.d);
        else if(msg.type == CHAR) Esp_Now::recieved_msgs.add<char>(msg.msg.c);
        else if(msg.type == BOOL) Esp_Now::recieved_msgs.add<bool>(msg.msg.b);
        else if(msg.type == DATA) Esp_Now::recieved_msgs.add<data>(msg.msg.dt);
    }else{
        if(msg.type == INT) Esp_Now::recieved_msgs.add<int>(msg.msg.i_ptr, msg.size);
        else if(msg.type == SHORT) Esp_Now::recieved_msgs.add<short>(msg.msg.s_ptr, msg.size);
        else if(msg.type == LONG) Esp_Now::recieved_msgs.add<long>(msg.msg.l_ptr, msg.size);
        else if(msg.type == FLOAT) Esp_Now::recieved_msgs.add<float>(msg.msg.f_ptr, msg.size);
        else if(msg.type == DOUBLE) Esp_Now::recieved_msgs.add<double>(msg.msg.d_ptr, msg.size);
        else if(msg.type == CHAR) Esp_Now::recieved_msgs.add<char>(msg.msg.c_ptr, msg.size);
        else if(msg.type == BOOL) Esp_Now::recieved_msgs.add<bool>(msg.msg.b_ptr, msg.size);
        else if(msg.type == DATA) Esp_Now::recieved_msgs.add<data>(msg.msg.dt_ptr, msg.size);
    }
}

void Esp_Now::OnDataSent(const uint8_t *mac_addr, Esp_Now_send_status_t status){
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == Esp_Now_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void Esp_Now::OnDataRecv(const Esp_Now_recv_info_t *info, const uint8_t *incomingData, int len){
    // Check if the received data length matches the size of the data struct
    // Create a temporary data struct to hold the received data
    msg_struct receivedData;
    // Copy the received data into the temporary struct
    memcpy(&receivedData, incomingData, sizeof(data));

    // Update the static variable to store the latest received message data
    Esp_Now::setRecvMsg(receivedData);
}

uint8_t Esp_Now::Local_MAC[MAC_LENGTH];

Msg_Queue Esp_Now::recieved_msgs;
/***********************************************************************/

/**************Constructors**************/
Esp_Now::Esp_Now(const COMMUNICATION communication, const int peers_crowd, const uint8_t* new_local_MAC){
    if(communication == SENDER || communication == RECEIVER || communication == TWO_WAY_COMMUNICATION){
        this->ESP_COM = communication;
    }else{
        this->setup_errors[this->error_counter] = Esp_Now_COMMUNICATION_SETUP_ERROR; 
        this->error_counter++;
    }

    this->ids = (int*)malloc(peers_crowd*sizeof(int));
    this->Peers_MAC = (uint8_t**)malloc(peers_crowd*sizeof(uint8_t*));
    for(int i=0; i<MAC_LENGTH; i++){
        this->Peers_MAC[i] = (uint8_t*)malloc(MAC_LENGTH*sizeof(uint8_t));
    }

    this->Encryption = false;

    memcpy(Esp_Now::Local_MAC, new_local_MAC, MAC_LENGTH);
}

Esp_Now::Esp_Now(const COMMUNICATION communication, const int peers_crowd, const uint8_t* new_local_MAC, const char* new_PMK_key){
    if(communication == SENDER || communication == RECEIVER || communication == TWO_WAY_COMMUNICATION){
        this->ESP_COM = communication;
    }else{
        this->setup_errors[this->error_counter] = Esp_Now_COMMUNICATION_SETUP_ERROR;
        this->error_counter++;
    }

    this->ids = (int*)malloc(peers_crowd*sizeof(int));
    this->Peers_MAC = (uint8_t**)malloc(peers_crowd*sizeof(uint8_t*));
    for(int i=0; i<MAC_LENGTH; i++){
        this->Peers_MAC[i] = (uint8_t*)malloc(MAC_LENGTH*sizeof(uint8_t));
    }

    this->Encryption = true;

    memcpy(Esp_Now::Local_MAC, new_local_MAC, MAC_LENGTH);

    if(this->PMK_key == nullptr){
        this->PMK_key = (char*)malloc((ENCRYPTION_KEY_LENGTH + 1) * sizeof(char));
    }

    if (this->PMK_key != nullptr) {
        // Copy the String content to PMK_key
        strcpy(this->PMK_key, new_PMK_key);
    } else {
        // Handle memory allocation failure
        this->setup_errors[this->error_counter] = MEMORY_ALLOCATION_ERROR;
        this->error_counter++;
    }
}
/***************************************/

/**************Starting of the espnow communication**************/
void Esp_Now::addPeer(int id, uint8_t* Peers_MAC, int Ch, wifi_interface_t mode){

    if(Ch < 0 || 13 < Ch){
        this->setup_errors[this->error_counter] = CHANNEL_OUT_OF_RANGRE; 
        this->error_counter++;
    }

    this->ids[this->id_counter] = id;
    memcpy(this->Peers_MAC[this->id_counter], Peers_MAC, MAC_LENGTH);
    this->id_counter++;

    // Initialize the peerInfo structure
    Esp_Now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, Peers_MAC, MAC_LENGTH);
    peerInfo.channel = Ch;  
    peerInfo.encrypt = false;
    peerInfo.ifidx = mode;  // Use station interface (most common for ESP-NOW)

    // Add receiver as peer        
    if (Esp_Now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("[Error] Failed to add peer");
        return;
    }else{
        Serial.println("[SUCCESS] peer has been added succesfuly");
    }
}





void Esp_Now::addPeer(int id, uint8_t* Peers_MAC, int Ch, wifi_interface_t mode, char* LMK_keys_array){
    if(Ch < 0 || 13 < Ch){
        this->setup_errors[this->error_counter] = CHANNEL_OUT_OF_RANGRE; 
        this->error_counter++;
    }

    this->ids[this->id_counter] = id;
    memcpy(this->Peers_MAC[this->id_counter], Peers_MAC, MAC_LENGTH);
    this->id_counter++;

    // Initialize the peerInfo structure
    Esp_Now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, Peers_MAC, MAC_LENGTH);
    peerInfo.channel = Ch;  
    peerInfo.encrypt = false;
    peerInfo.ifidx = mode;  // Use station interface (most common for ESP-NOW)

    //Set the receiver device LMK key
    for (uint8_t i = 0; i < 16; i++) {
        peerInfo.lmk[i] = LMK_keys_array[i];
    }


    // Add receiver as peer        
    if (Esp_Now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }else{
        Serial.println("[SUCCESS] peer has been added succesfuly");
    }
}


void Esp_Now::addPeer(int id, Esp_Now_peer_info_t* Peer){
    this->ids[this->id_counter] = id;
    memcpy(this->Peers_MAC[this->id_counter], Peer->peer_addr, MAC_LENGTH);
    this->id_counter++;

    // Add receiver as peer        
    if (Esp_Now_add_peer(Peer) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }else{
        Serial.println("[SUCCESS] peer has been added succesfuly");
    }
}

void Esp_Now::begin(){
    // Set the WiFi module to station mode
    WiFi.mode(WIFI_STA);

    if (Esp_Now_init() != ESP_OK) {
        this->setup_errors[this->error_counter] = Esp_Now_INITIALIZATION_ERROR; 
        this->error_counter++;
    }

    // Setup communication based on mode
    switch(this->ESP_COM) {
        case SENDER:
            // Register for Send CB to get the status of Transmitted packet
            Esp_Now_register_send_cb(Esp_Now::OnDataSent);
            break;
        case RECEIVER:
            Esp_Now_register_recv_cb(Esp_Now::OnDataRecv);
            break;
        case TWO_WAY_COMMUNICATION:
            Esp_Now_register_send_cb(Esp_Now::OnDataSent);
            Esp_Now_register_recv_cb(Esp_Now::OnDataRecv);
            break;
    }


    // Read the old MAC
    Serial.print("[OLD] ESP32 Board MAC Address:  ");
    Serial.println(WiFi.macAddress());

    delay(100);

    // Set new MAC
    esp_wifi_set_mac(WIFI_IF_STA, this->Local_MAC);

    delay(100);
    
    Serial.print("\r[NEW] ESP32 Board MAC Address:  ");
    Serial.println(WiFi.macAddress());

    // Verify that the new MAC is set correctly

    if (!WiFi.macAddress().equals(getMACtoSTRING(this->Local_MAC))) {
        Serial.println("\r[Error] Failed to change MAC");
        this->setup_errors[this->error_counter] = NEW_MAC_INITIALIZATION_ERROR;
        this->error_counter++;
    }

    if(this->Encryption){
        Esp_Now_set_pmk((uint8_t *)this->PMK_key);
    }
    delay(100);
}
/**********************************************************/

void Esp_Now::setChannel(int ch){
    WiFi.setChannel(ch);
    delay(100);
}
/**************Checking for istalisation errors**************/
bool Esp_Now::FAIL_CHECK() {
    if(this->error_counter == 0) {
        Serial.println("\r[SUCCESS] THERE WERE NO INITIALIZATION ERROR");
        return false;
    }
    for(int i = 0; i < this->error_counter; i++) {
        if(this->setup_errors[i]==Esp_Now_INITIALIZATION_ERROR) Serial.println("\r[Error] initializing ESP-NOW");
        else if(this->setup_errors[i]==Esp_Now_COMMUNICATION_SETUP_ERROR ) Serial.println("\r[Error] in the consructors communication parameter");
        else if(this->setup_errors[i]==CHANNEL_OUT_OF_RANGRE) Serial.println("\r[Error] in the value of the channel (channel ranges 0-13)");        
        else if(this->setup_errors[i]==NEW_MAC_INITIALIZATION_ERROR) Serial.println("\r[Error] setting new MAC address");
        else if(this->setup_errors[i]==ADD_PEER_INITIALIZATION_ERROR) Serial.println("\r[Error] adding peer");
        else if(this->setup_errors[i]==MEMORY_ALLOCATION_ERROR) Serial.println("\r[Error] allocating memory");
    }
    Serial.println();
    return true;
}
/***********************************************************/

/**************Checking if the esp has recieved any msg**************/
bool Esp_Now::available() const{
    return !this->recieved_msgs.isEmpty();
}

bool Esp_Now::isArray() const{
    return Esp_Now::recieved_msgs.isFrontArray();
}

MSG_VARIABLE_TYPE Esp_Now::data_type() const{
    return Esp_Now::recieved_msgs.data_type();
}
/********************************************************************/

void Esp_Now::setWiFi_to_STA(){
    WiFi.mode(WIFI_MODE_STA);
    delay(10);
}

// Set WiFi to AP mode
void Esp_Now::setWiFi_to_AP() {
    WiFi.mode(WIFI_MODE_AP);
    delay(10);  
}

// Set WiFi to AP+STA mode
void Esp_Now::setWiFi_to_APSTA() {
    WiFi.mode(WIFI_MODE_APSTA);
    delay(10);
}

Esp_Now::~Esp_Now(){
    Esp_Now::recieved_msgs.~Msg_Queue();
    for(int i=0; i<this->id_counter; i++){
        free(this->Peers_MAC[i]);
    }
    free(this->Peers_MAC);
    free(this->PMK_key);
    free(ids);

    for(int i=0; i<this->id_counter; i++){
        Esp_Now_del_peer(this->Peers_MAC[i]);
        delay(10);
    }
    Esp_Now_deinit();
    
    Esp_Now::recieved_msgs.~Msg_Queue();
    Esp_Now::recieved_msgs = Msg_Queue();
}


void Esp_Now::setCustomSendCallback(Esp_Now_send_cb_t custom){
    Esp_Now_unregister_send_cb();
    Esp_Now_register_send_cb(custom);
}

void Esp_Now::setCustomRecvCallback(Esp_Now_recv_cb_t custom){
    Esp_Now_unregister_recv_cb();
    Esp_Now_register_recv_cb(custom);
}

