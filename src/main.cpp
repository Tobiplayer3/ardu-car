#include <Arduino.h>
#include <Stepper.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0){
    Serial.read();
  }
}
