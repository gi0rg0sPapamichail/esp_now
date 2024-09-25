# ESP_NOW Library

## Overview

This library helps initialize the ESP-NOW protocol, utilizing all its important tools. It addresses common issues that may be overlooked. If you are unfamiliar with the ESP-NOW protocol and its applications, this library provides the guidance you need to use it to its fullest potential.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Examples](#examples)
- [API Reference](#api-reference)
- [Changelog](#Changelog)
- [License](#license)

## Installation

To install the ESP_NOW library, follow these steps:

1. Download the latest release from the [releases page]([https://github.com/gi0rg0sPapamichail/ESP_NOW_H](https://github.com/gi0rg0sPapamichail/esp_now)).
2. Open the Arduino IDE.
3. Go to **Sketch** > **Include Library** > **Add .Zip library...**
4. Search for "ESP_NOW-main" and click **Open**.

## Usage

1. Include the library in your sketch: `#include <ESP_NOW.h>`
2. Initialize an `ESP_NOW` object with desired communication parameters.
3. Note that the Serial.begin() must always be called for the library to work before the object initialization.
4. Call  the `begin()` method to start the protocol.
5. Call the apropriate `addPeer()` method to give the information of the peer.

```cpp
#include <ESP_NOW.h>

#define PEERS 3

uint8_t MAC[MAC_LENGTH] = {/*Your MAC adress*/};
uint8_t PEERS_MAC[PEERS][MAC_LENGTH] = {
    {/*Your peers MAC adress*/},
    {/*Your peers MAC adress*/},
    {/*Your peers MAC adress*/}
    };

// Initialize ESP_NOW object
ESP_NOW myesp(SENDER, PEERS, MAC);

void setup() {
    Serial.begin(115200);
    
    // Initialize ESP-NOW
    myesp.begin();

    myesp.addPeer(0, PEERS_MAC[0], 0, WIFI_IF_STA);
    myesp.addPeer(1, PEERS_MAC[1], 0, WIFI_IF_STA);
    myesp.addPeer(2, PEERS_MAC[2], 0, WIFI_IF_STA);

    // Check for initialization errors
    myesp.FAIL_CHECK();
}

void loop() {
    // Your code here
}
```

## Examples

### SENDER EXAMPLE
```cpp
#include "ESP_NOW.h"


uint8_t MACS[4][MAC_LENGTH] = {
  {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA},
  {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAB},
  {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAC},
  {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAD}
};

uint8_t Senders_MAC[MAC_LENGTH] = {0xCA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

ESP_NOW my_esp(SENDER, 4, Senders_MAC);

void setup() {
  Serial.begin(115200);

  // delay(500);
  my_esp.begin();
  my_esp.addPeer(0, MACS[0], 0, WIFI_IF_STA);
  my_esp.addPeer(1, MACS[1], 0, WIFI_IF_STA);
  my_esp.addPeer(2, MACS[2], 0, WIFI_IF_STA);
  my_esp.addPeer(3, MACS[3], 0, WIFI_IF_STA);
  my_esp.FAIL_CHECK();
}

void loop() {
  my_esp.Send(0, 1);\\ sends to esp that was assigned id:0 in the addPeer method
  delay(1000);
  my_esp.Send(1, 1);
  delay(1000);
  my_esp.Send(2, 1);
  delay(1000);
  my_esp.Send(3, 1);
  delay(1000);
  my_esp.Send(2, 1);
  delay(1000);
  my_esp.Send(1, 1);
  delay(1000);
  my_esp.Send(0, 1);
  delay(1000);
  my_esp.Send(3, 5);
  delay(5000);
}

```

### RECIEVER CODE
```cpp
#include "ESP_NOW.h"

#define ID 3

int timer = 5;
int led = 0;

uint8_t MACS[4][MAC_LENGTH] = {
  {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA},
  {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAB},
  {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAC},
  {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAD}
};

uint8_t senders_MAC[MAC_LENGTH] = {0xFF, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

ESP_NOW my_esp(RECEIVER, 1, MACS[ID]);

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);

  my_esp.begin();
  // delay(500);
  my_esp.addPeer(0, senders_MAC, 0, WIFI_IF_STA);
  my_esp.FAIL_CHECK();
  // my_esp.FAIL_CHECK();
}

void loop() {
  if(my_esp.available()){
    timer = my_esp.read<int>();
  }

  if(timer > 0){
    digitalWrite(led, HIGH);
    sleep(timer);
    digitalWrite(led, LOW);
    timer = 0;
  }
}
```

## API Reference
### Class: `ESP_NOW`
A class to manage ESP-NOW communication, providing functionalities for sending and receiving messages, managing peers, and handling encryption.


### **Constructor**
- **`ESP_NOW(const COMMUNICATION communication, const int peers_crowd, const uint8_t* new_local_MAC)`**
    - **Parameters**:
        - `communication`: Mode of communication (SENDER, RECEIVER, or TWO_WAY_COMMUNICATION).
        - `peers_crowd`: Number of peers to manage.
        - `new_local_MAC`: Pointer to the new local MAC address.
    - **Description**: Initializes the ESP_NOW instance with specified communication mode and peer configuration.

- **`ESP_NOW(const COMMUNICATION communication, const int peers_crowd, const uint8_t* new_local_MAC, const char* new_PMK_key)`**
    - **Parameters**:
        - `communication`: Mode of communication.
        - `peers_crowd`: Number of peers.
        - `new_local_MAC`: Pointer to the new local MAC address.
        - `new_PMK_key`: Pointer to the new PMK encryption key.
    - **Description**: Initializes the instance with encryption support.

---

### **Methods**
- **`void begin()`**
    - **Description**: Initializes the ESP-NOW communication and sets the MAC address.

- **`void addPeer(int id, uint8_t* Peers_MAC, int Ch, wifi_interface_t mode)`**
    - **Parameters**:
        - `id`: Unique identifier for the peer.
        - `Peers_MAC`: Pointer to the MAC address of the peer.
        - `Ch`: Channel to communicate on (0-13).
        - `mode`: WiFi interface (WIFI_IF_STA or WIFI_IF_AP).
    - **Description**: Adds a peer for communication.

- **`void addPeer(int id, uint8_t* Peers_MAC, int Ch, wifi_interface_t mode, char* LMK_keys_array)`**
    - **Parameters**:
        - Same as the previous method, with an additional `LMK_keys_array` parameter for encryption.
    - **Description**: Adds a peer with encryption.

- **`void addPeer(int id, esp_now_peer_info_t* Peer)`**
    - **Parameters**:
        - `id`: Unique identifier for the peer.
        - `Peer`: Pointer to the peer information structure.
    - **Description**: Adds a peer using the provided peer info.

- **`void Send(const int id, const data msg)`**
    - **Parameters**:
        - `id`: Identifier of the target peer.
        - `msg`: Data to be sent.
    - **Description**: Sends a message to a specified peer.

- **`bool FAIL_CHECK()`**
    - **Description**: Checks for initialization errors and prints error messages.

- **`bool available() const`**
    - **Description**: Checks if there are any received messages available.

- **`bool isArray() const`**
    - **Description**: Checks if the last received message is an array.

- **`MSG_VARIABLE_TYPE data_type() const`**
    - **Description**: Returns the type of the last received message.

- **`void setChannel(int ch)`**
    - **Parameters**:
        - `ch`: Channel number to set (0-13).
    - **Description**: Sets the WiFi channel for communication.

- **`void setWiFi_to_STA()`**
    - **Description**: Sets WiFi mode to station.

- **`void setWiFi_to_AP()`**
    - **Description**: Sets WiFi mode to access point.

- **`void setWiFi_to_APSTA()`**
    - **Description**: Sets WiFi mode to access point + station.

- **`void setCustomSendCallback(esp_now_send_cb_t custom)`**
    - **Parameters**:
        - `custom`: Custom callback function for sending data.
    - **Description**: Registers a custom send callback.

- **`void setCustomRecvCallback(esp_now_recv_cb_t custom)`**
    - **Parameters**:
        - `custom`: Custom callback function for receiving data.
    - **Description**: Registers a custom receive callback.

---

### **Destructor**
- **`~ESP_NOW()`**
    - **Description**: Cleans up resources and deinitializes ESP-NOW.

---


## Changelog

### Enhancements
- **Improved Peer Management**: Fixed issues with adding multiple peers, ensuring stable and reliable communication.
- **Generic Send Function**: Refactored the `Send` function to be more flexible, allowing any data type to be sent using a single function (`Send`) instead of separate functions for each type.
- **Custom Queue Implementation**: Introduced a dynamic message queue for storing messages, optimizing memory usage.
- **User-defined Callbacks**: Added functionality for users to set their own custom data send and receive functions, increasing flexibility.
- **Encryption Enhancements**: Fixed encryption problems and added support for both PMK and LMK keys to improve security.
- **Support for AP and STA Modes**: Enhanced the class to support both Station (STA) and Access Point (AP) modes, which were previously limited to STA only.
- **Channel Communication Fixes**: Correctly implemented message sending across different channels, not just channel 1.

### Bug Fixes
- Resolved issues with sending messages to peers on channels other than channel 1.
- Fixed encryption issues to ensure proper data security.

## License

MIT License

Copyright (c) 2024 GEORGE PAPAMICHAIL

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

The author of this software shall not be held liable for any damages, liabilities, or legal consequences
arising from the use, misuse, or inability to use the software.
