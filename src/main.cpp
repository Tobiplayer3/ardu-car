#include <Arduino.h>
#include <Stepper.h>

bool on = false;
int steps = 1024;
bool automatic = false;
bool left = true;
bool right = true;
int rpm = 20;
char nextAction;

Stepper stepperR = Stepper(steps, 7, 9, 6, 8);
Stepper stepperL = Stepper(steps, 4, 6, 5, 7);

int ultraSonicR = 2;
int ultraSonicL = 4;
int ultraSonicF = 5;
int ultraSonicTrigger = 3;

enum direction { LEFT, RIGHT, FRONT };

// declare methods to access them
float getDistance(direction dir);
direction getHighestDistance();
void drive(int steps, direction dir);
void rotate(direction dir);
int getUltraSonic(direction dir);
bool isDistanceTooLow(direction dir, int maxDistance);
void setSpeed();

void setup() {
  Serial.begin(9600);
  setSpeed();
  pinMode(ultraSonicR, INPUT);
  pinMode(ultraSonicL, INPUT);
  pinMode(ultraSonicF, INPUT);
  pinMode(ultraSonicTrigger, OUTPUT);
}

void loop() {
  // check for Serial inputs
  if (Serial.available() > 0) {
    nextAction = Serial.read();

    /*
      0: toggle status
      1: toggle automatic
      2: increase speed
      3: decrease speed

      manual:
      4: forward
      5: right
      6: left
      7: backward
    */
    switch (nextAction) {
      case '0':
        on = !on;
        break;
      case '1':
        automatic = !automatic;
        break;
      case '2':
        rpm += 1;
        setSpeed();
        break;
      case '3':
        rpm -= 1;
        setSpeed();
        break;
    }
  }

  // handle driving
  if (on) {
    // automatic driving
    if (automatic) {
      drive(1, FRONT);
      if(isDistanceTooLow(FRONT, 5)){
        direction dir = getHighestDistance();
        if(!isDistanceTooLow(dir, 5)){
          rotate(dir);
          drive(1, FRONT);
          rotate(FRONT);
        }
      }else if(isDistanceTooLow(RIGHT, 5)){

      }else if(isDistanceTooLow(LEFT, 5)){

      }
      // TODO

    // manual driving
    } else {
      switch (nextAction) {
        case '4':
          drive(1, FRONT);
          break;
        case '5':
          drive(1, RIGHT);
          break;
        case '6':
          drive(1, LEFT);
          break;
        case '7':
          drive(-1, FRONT);
          break;
      }
    }
  }
}

// drive a distance in steps
void drive(int steps, direction dir) {
  //drive backwards if steps are negative
  bool forward = true;
  if(steps > 0){
    forward = false;
    steps = abs(steps);
  }

  rotate(dir);

  for (int i = 0; i < steps; i++) {
    if (forward) {
      if (left) {
        stepperL.step(1);
      }
      if (right) {
        stepperR.step(1);
      }
    } else {
      if (left) {
        stepperL.step(-1);
      }
      if (right) {
        stepperR.step(-1);
      }
    }
  }

  rotate(FRONT);

}

// rotate by blocking selected tires
void rotate(direction dir) {
  switch (dir) {
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

// return right or left depending on the highest distance
direction getHighestDistance() {
  float distanceR = getDistance(RIGHT);
  float distanceL = getDistance(LEFT);

  if (distanceR >= distanceL) {
    return RIGHT;
  } else {
    return LEFT;
  }
}

// check if the distance between the ultrasonic and the wall is too low
bool isDistanceTooLow(direction dir, int maxDistance) {
  float distance = getDistance(dir);
  if (distance < maxDistance) {
    return true;
  }
  return false;
}

// get distance from ultrasonic in cm
float getDistance(direction dir) {
  int ultraSonic = getUltraSonic(dir);
  digitalWrite(ultraSonicTrigger, LOW);
  digitalWrite(ultraSonicTrigger, HIGH);
  digitalWrite(ultraSonicTrigger, LOW);

  float duration = pulseIn(ultraSonic, HIGH);
  return (duration / 2) * 0.03432;
}

// returns the pin of the ultrasonic
int getUltraSonic(direction dir) {
  switch (dir) {
    case RIGHT:
      return ultraSonicR;
    case LEFT:
      return ultraSonicL;
    case FRONT:
      return ultraSonicF;
  }
}

void setSpeed(){
  stepperL.setSpeed(rpm);
  stepperR.setSpeed(rpm);
}
