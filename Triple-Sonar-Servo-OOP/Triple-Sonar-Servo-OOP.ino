#include <Arduino.h>
#include "Sonar.h"
#include <Servo.h>

SonarModule sonarModule;
Servo myservo;

int it = 1; 
float localDeclinationAngle = 0.0; 
float compassAngle;
int logNumber = 1; 
int sonarFrontMemory[90];
int sonarLeftMemory[90];
int sonarRightMemory[90];
int startAngleMemory[4];
int cumulativeAngleMemory[30];
int pos = 0;

unsigned long previousMillisServo = 0;
const long intervalServo = 3; // interval for servo movement
unsigned long previousMillisSonar = 0;
const long intervalSonar = 50; // interval for sonar reading
bool increasing = true;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  myservo.attach(6);  
  Serial.println("Servo Setup complete");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Servo Control
  if (currentMillis - previousMillisServo >= intervalServo) {
    previousMillisServo = currentMillis;
    
    if (increasing) {
      pos++;
      if (pos >= 90) {
        increasing = false;
      }
    } else {
      pos--;
      if (pos <= 40) {
        increasing = true;
      }
    }
    myservo.write(pos);
  }

  // Sonar Reading
  if (currentMillis - previousMillisSonar >= intervalSonar) {
    previousMillisSonar = currentMillis;
    PrintSerial(pos);
  }
}

int PrintSerial(int i){
  int sonarFront = sonarModule.kalmanFilter(0);
  int sonarLeft = sonarModule.kalmanFilter(1);
  int sonarRight = sonarModule.kalmanFilter(2);

  Serial.print(i);
  Serial.print(",");
  Serial.print(sonarFront);
  Serial.print(",");
  Serial.print(sonarFront);
  Serial.print(",");
  Serial.print(sonarLeft);
  Serial.print(",");
  Serial.print(sonarRight);
  Serial.print(",");
  Serial.println("1");
}
