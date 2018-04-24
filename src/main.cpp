#include <Arduino.h>
#include <Stepper.h>

bool on = false;
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

// declare methods to access them
float getDistance(int id);
int getHighestDistance();
void drive(int steps);
void rotate(int degree);
int* getUltraSonicByID(int id);
bool isDistanceTooLow(int id);

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0){
    Serial.read();

    if(automatic && on){
      if(isDistanceTooLow(0)){

      }else if(isDistanceTooLow(1)){

      }else if(isDistanceTooLow(2)){
        if(getDistance(0) >= getDistance(1)){
          rotate(90);
        }else{
          rotate(-90);
        }
      }

      drive(steps);

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
void rotate(int degree){

}

int getHighestDistance(){
  float distanceR = getDistance(0);
  float distanceL = getDistance(1);
  float distanceF = getDistance(2);

  if(distanceR >= distanceL && distanceR >= distanceF){
    return 0;
  }else if(distanceL >= distanceR && distanceL >= distanceF){
    return 1;
  }else{
    return 2;
  }
}

// check if the distance between the ultrasonic and the wall is too low
bool isDistanceTooLow(int id){
  float distance = getDistance(id);
  if(distance < 5){
    return true;
  }
  return false;
}

// get distance from ultrasonic in cm
float getDistance(int id){
  int* ultraSonic = getUltraSonicByID(id);
  digitalWrite(ultraSonic[0], LOW);
  digitalWrite(ultraSonic[0], HIGH);
  digitalWrite(ultraSonic[0], LOW);

  float duration = pulseIn(ultraSonic[1], HIGH);
  return (duration/2) * 0.03432;
}

int* getUltraSonicByID(int id){
  switch (id) {
    case 0: return ultraSonicR;
    case 1: return ultraSonicL;
    case 2: return ultraSonicF;
  }
}
