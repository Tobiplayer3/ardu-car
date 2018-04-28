#include <Arduino.h>
#include <Stepper.h>

bool on = false;
int steps = 100;
bool automatic = true;
bool left = true;
bool right = true;
int rpm = 30;

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
  stepperL.setSpeed(rpm);
  stepperR.setSpeed(rpm);
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
  int nextAction = 0;

  // check for Serial inputs
  if(Serial.available() > 0){
    nextAction = Serial.read();

    /*
    0: turn on
    1: set automatic
    2: set manual
    3: set speed

    manual:
    4: forwards
    5: right
    6: left
    */
    switch(nextAction){
      case 0:
        on = true;
        break;
      case 1:
        automatic = true;
        break;
      case 2:
        automatic = false;
        break;
      case 3:
        rpm = Serial.read();
        stepperL.setSpeed(rpm);
        stepperR.setSpeed(rpm);
        break;
    }
  }

  // handle driving
  if(on){
    // automatic driving
    if(automatic){
      drive(100);
      // TODO

    // manual driving
    }else{
      switch (nextAction) {
        case 4:
          drive(10);
          break;
        case 5:
          rotate(RIGHT);
          drive(10);
          rotate(FRONT);
          break;
        case 6:
          rotate(LEFT);
          drive(10);
          rotate(FRONT);
          break;
      }
    }
  }
}

// drive a distance in steps
void drive(int steps){
  for(int i = 0; i < steps; i++){
    if(left){
      stepperL.step(1);
    }
    if(right){
      stepperR.step(1);
    }
  }
}

// rotate
void rotate(direction direction){
  switch(direction) {
    case LEFT:
      left = false;
      break;
    case RIGHT:
      right = false;
      break;
    case FRONT:
      right = true;
      left = true;
      break;
  }
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
