#include <Arduino.h>

#define BAUD_RATE 115200

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
}

void loop() {

  int cb = Serial.available();
  if (cb) {
    Serial.readStringUntil('<');
    String cmd = Serial.readStringUntil('>');
    if (cmd == "BucketDao::listAll") {

      int i = 0;
      while(1) {
        delay(1000);
        Serial.print(
          String("Bucket #") + String(i++) + String(",") + 
          String("Bucket #") + String(i++) + String(",") + 
          String("Bucket #") + String(i++) + String(",") + 
          String("Bucket #") + String(i++) + String(",") + 
          String("Bucket #") + String(i++) + String(",") + 
          String("Bucket #") + String(i++) + String(",") + 
          String("Bucket #") + String(i++) + String(",") + 
          String("Bucket #") + String(i++) + String(",") + 
          String("Bucket #") + String(i++) + String(",") + 
          String("Bucket #") + String(i++)
        );
      }

    }
  }
}