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

void setup() {
  Serial.begin(9600);
  while (!Serial);

  myservo.attach(6);  
  Serial.println("Servo Setup complete");
}

void loop() {
  int currentAngle;
  int startAngle;
  int stopAngle;
  int angleRange[75];
  static int address = 0;
  static int send = 0;
  static int address1 = 0;
  static int address2 = 0;
  static int diff = 0;

  // oscillate myservo between 0 and 90
  for (pos = 40; pos <= 90; pos += 1) {
    myservo.write(pos);
    delay(5);
  }

  PrintSerial();

  for (pos = 90; pos >= 40; pos -= 1) {
    myservo.write(pos);
    delay(5);
  }

  address2 = address;
  send = 0;
}

int PrintSerial(){
  int sonarFront = sonarModule.kalmanFilter(0);
  int sonarLeft = sonarModule.kalmanFilter(1);
  int sonarRight = sonarModule.kalmanFilter(2);

  // Serial.print();
  // Serial.print(",");
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
