# ESP_NOW_HR Library

The ESP_NOW_HR library facilitates communication using ESP-NOW protocol on ESP32 boards.

## Installation

1. Download the library.
2. Extract the zip file and place the `esp_now_HR` folder in your Arduino libraries directory.
3. Restart the Arduino IDE.

## Usage

1. Include the library in your sketch: `#include <esp_now_HR.h>`
2. Initialize an `ESP_NOW_HR` object with desired communication parameters.
3. Call appropriate methods to send or receive data using ESP-NOW.
4. Note that the Serial.begin() must always be called for the library to work before the object initialization.

```cpp
#include <esp_now_HR.h>

// Initialize ESP_NOW_HR object
ESP_NOW_HR esp_now_hr(SENDER, 0, mac_address, false);

void setup() {
    Serial.begin(115200);
    
    // Initialize ESP-NOW
    esp_now_hr.begin();

    // Check for initialization errors
    esp_now_hr.FAIL_CHECK();
}

void loop() {
    // Your code here
}
