#include <Arduino.h>
#include <Stepper.h>

bool on = false;
int steps = 1024;
bool automatic = false;
bool left = true;
bool right = true;
int rpm = 20;

Stepper stepperR = Stepper(steps, 7, 9, 6, 8);
Stepper stepperL = Stepper(steps, 4, 6, 5, 7);

int ultraSonicR = 2;
int ultraSonicL = 4;
int ultraSonicF = 5;
int ultraSonicTrigger = 3;

enum direction { LEFT, RIGHT, FRONT };

// declare methods to access them
float getDistance(direction direction);
direction getHighestDistance();
void drive(int steps, bool forward);
void rotate(direction direction);
int getUltraSonic(direction direction);
bool isDistanceTooLow(direction direction);

void setup() {
  Serial.begin(9600);
  stepperL.setSpeed(rpm);
  stepperR.setSpeed(rpm);
  pinMode(ultraSonicR, INPUT);
  pinMode(ultraSonicL, INPUT);
  pinMode(ultraSonicF, INPUT);
  pinMode(ultraSonicTrigger, OUTPUT);
}

void loop() {
  int nextAction = 0;

  // check for Serial inputs
  if (Serial.available() > 0) {
    nextAction = Serial.read();

    /*
    0: turn on
    1: set automatic
    2: set manual
    3: set speed

    manual:
    4: forward
    5: right
    6: left
    */
    switch (nextAction) {
    case 0:
      on = true;
      break;
    case 1:
      automatic = true;
      break;
    case 2:
      automatic = false;
      break;
    }
  }

  // handle driving
  if (on) {
    // automatic driving
    if (automatic) {
      drive(100, true);
      // TODO

      // manual driving
    } else {
      switch (nextAction) {
      case 4:
        drive(1024, true);
        break;
      case 5:
        rotate(RIGHT);
        drive(1024, true);
        rotate(FRONT);
        break;
      case 6:
        rotate(LEFT);
        drive(1024, true);
        rotate(FRONT);
        break;
      case 7:
        drive(1024, false);
        break;
      }
    }
  }
}

// drive a distance in steps
void drive(int steps, bool forward) {
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
}

// rotate
void rotate(direction direction) {
  switch (direction) {
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

direction getHighestDistance() {
  float distanceR = getDistance(RIGHT);
  float distanceL = getDistance(LEFT);
  float distanceF = getDistance(FRONT);

  if (distanceR >= distanceL && distanceR >= distanceF) {
    return RIGHT;
  } else if (distanceL >= distanceR && distanceL >= distanceF) {
    return LEFT;
  } else {
    return FRONT;
  }
}

// check if the distance between the ultrasonic and the wall is too low
bool isDistanceTooLow(direction direction) {
  float distance = getDistance(direction);
  if (distance < 5) {
    return true;
  }
  return false;
}

// get distance from ultrasonic in cm
float getDistance(direction direction) {
  int ultraSonic = getUltraSonic(direction);
  digitalWrite(ultraSonicTrigger, LOW);
  digitalWrite(ultraSonicTrigger, HIGH);
  digitalWrite(ultraSonicTrigger, LOW);

  float duration = pulseIn(ultraSonic, HIGH);
  return (duration / 2) * 0.03432;
}

int getUltraSonic(direction direction) {
  switch (direction) {
  case RIGHT:
    return ultraSonicR;
  case LEFT:
    return ultraSonicL;
  case FRONT:
    return ultraSonicF;
  }
}
