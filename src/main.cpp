#include <Arduino.h>
#include <Stepper.h>

int steps = 100;

int motorR[] = {0,2,1,3};
Stepper stepperR = Stepper(steps, motorR[0], motorR[1], motorR[2], motorR[3]);

int motorL[] = {4,6,5,7};
Stepper stepperL = Stepper(steps, motorL[0], motorL[1], motorL[2], motorL[3]);

// ultrasonic 0
int ultraSonicR[] = {4,5};
// ultrasonic 1
int ultraSonicL[] = {6,7};
// ultrasonic 2
int ultraSonicF[] = {8,9};

bool automatic = true;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0){
    Serial.read();

    if(automatic){

    }else{

    }

  }
}

// drive a distance in steps
void drive(int steps){
  for(int i = 0; i > steps; i++){
    stepperL.step(1);
    stepperR.step(1);
  }
}

// rotate in degree
void rotate(int direction, int degree){

}

int getHighestDistance(){
  float distanceR = getDistance(ultraSonicR);
  float distanceL = getDistance(ultraSonicL);
  float distanceF = getDistance(ultraSonicF);

  if(distanceR >= distanceL && distanceR >= distanceF){
    return 0;
  }else if(distanceL >= distanceR && distanceL >= distanceF){
    return 1;
  }else{
    return 2;
  }
}

// get distance from ultrasonic in cm
float getDistance(int ultraSonic[]){
  digitalWrite(ultraSonic[0], LOW);
  digitalWrite(ultraSonic[0], HIGH);
  digitalWrite(ultraSonic[0], LOW);

  float duration = pulseIn(ultraSonic[1], HIGH);
  return (duration/2) * 0.03432;
}
