#include <Arduino.h>
#include <Stepper.h>

bool on = false;
int steps = 100;
bool automatic = true;

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

enum direction {
  LEFT,
  RIGHT,
  FRONT
};

// declare methods to access them
float getDistance(direction direction);
direction getHighestDistance();
void drive(int steps);
void rotate(int degree);
int* getUltraSonic(direction direction);
bool isDistanceTooLow(direction direction);

void setup() {
  Serial.begin(9600);
}

void test(){
  drive(100);
  Serial.print("Right: ");
  Serial.println(getDistance(RIGHT));
  Serial.print("Left: ");
  Serial.println(getDistance(LEFT));
  Serial.print("Front: ");
  Serial.println(getDistance(FRONT));
}

void loop() {
  while(Serial.available() > 0){
    int input = Serial.read();

    /*
    0: turn on
    1: set automatic
    2: set manual

    manual:
    3: forwards
    4: right
    5: left
    */
    switch(input){
      case 0: on = true;
      case 1: automatic = true;
              break;
      case 2: automatic = false;
              break;
      case 3: break;
      case 4: break;
      case 5: break;
      default: break;
    }
  }

  if(on){
    if(automatic){
      drive(100);
    }else{

    }
  }

}

// drive a distance in steps
void drive(int steps){
  for(int i = 0; i < steps; i++){
    stepperL.step(1);
    stepperR.step(1);
  }
}

// rotate in degree, probably useless
void rotate(int degree){

}

direction getHighestDistance(){
  float distanceR = getDistance(RIGHT);
  float distanceL = getDistance(LEFT);
  float distanceF = getDistance(FRONT);

  if(distanceR >= distanceL && distanceR >= distanceF){
    return RIGHT;
  }else if(distanceL >= distanceR && distanceL >= distanceF){
    return LEFT;
  }else{
    return FRONT;
  }
}

// check if the distance between the ultrasonic and the wall is too low
bool isDistanceTooLow(direction direction){
  float distance = getDistance(direction);
  if(distance < 5){
    return true;
  }
  return false;
}

// get distance from ultrasonic in cm
float getDistance(direction direction){
  int* ultraSonic = getUltraSonic(direction);
  digitalWrite(ultraSonic[0], LOW);
  digitalWrite(ultraSonic[0], HIGH);
  digitalWrite(ultraSonic[0], LOW);

  float duration = pulseIn(ultraSonic[1], HIGH);
  return (duration/2) * 0.03432;
}

int* getUltraSonic(direction direction){
  switch (direction) {
    case RIGHT: return ultraSonicR;
    case LEFT: return ultraSonicL;
    case FRONT: return ultraSonicF;
  }
}
