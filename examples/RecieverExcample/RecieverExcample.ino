#include "esp_now_HR.h"
#include <Adafruit_NeoPixel.h>

// Define the number of NeoPixels and the pin number
#define NUMPIXELS 1 // Number of pixels in the strip
#define PIN 38        // Pin number where the data line is connected

#define CHANNEL 0

bool encryption = false;

uint8_t new_MAC[MAC_LENGTH] =  {0xFA, 0xFF, 0xA6, 0xAA, 0xB1, 0x04};


ESP_HR myesp(RECIEVER, CHANNEL, new_MAC, encryption);
// Create a NeoPixel object
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin();
  myesp.begin();
  pixels.begin();  
  pixels.setBrightness(255);
}

void loop() {
  if(myesp.available()){
    DATA msg_recv = myesp.getLatestMsg();    
    if(msg_recv.type == 'I'){
      int msg_int = msg_recv.msg_int;
    }
    switch(msg_int){
      case 0:
        pixels.setPixelColor(0, (0,25,200));
        break;
      case 1:
        pixels.setPixelColor(0, (0,25,0));
        break;
      case 2:
        pixels.setPixelColor(0, (25,25,));
        break;
      case 3:
        pixels.setPixelColor(0, (25,25,25));
        break;
      case 4:
        pixels.setPixelColor(0, (0,0,0));
        break;
    }
    pixels.show();
  }
  delay(10);
}
