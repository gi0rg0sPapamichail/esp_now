#include "esp_now_HR.h"

#define CHANNEL 0

bool encryption = false;

uint8_t new_MAC[MAC_LENGTH] =  {0xFA, 0xFF, 0xA6, 0xAA, 0xB1, 0x04};


ESP_HR myesp(SENDER, CHANNEL, new_MAC, encryption);


void setup() {
  Serial.begin();
  myesp.begin();
}

void loop() {
  if(Serial.available()>0){
    String read = Serial.readStringUntil('\n');
    int msg = read - '0';
    if(0 <= msg <= 4){
      myesp.SendInt(msg);
    }
  }
  delay(10);
}