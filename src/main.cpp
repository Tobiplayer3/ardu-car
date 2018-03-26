#include <Arduino.h>
#include <Stepper.h>

int motorR1 = 0;
int motorR2 = 1;
Stepper stepperR = Stepper(100, motorR1, motorR2);

int motorL1 = 2;
int motorL2 = 3;
Stepper stepperL = Stepper(100, motorL1, motorL2);

int ultraSonicRIn = 4;
int ultraSonicROut = 5;

int ultraSonicLIn = 6;
int ultraSonicLOut = 7;

int ultraSonicFIn = 8;
int ultraSonicFOut = 9;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0){
    Serial.read();
  }
}

// drive a distance in cm
void drive(int distance){

}
