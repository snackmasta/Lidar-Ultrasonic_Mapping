#include <Arduino.h>
#include "Sonar.h"
#include <Servo.h>

SonarModule sonarModule;

Servo myservo1;  // create Servo1
Servo myservo2;  // create Servo2
Servo myservo3;  // create Servo3

int it = 1; // Number of iterations for averaging distance
float localDeclinationAngle = 0.0; 
float compassAngle;
int logNumber = 1; // Initialize logNumber to 1
int sonarFrontMemory[90];
int sonarLeftMemory[90];
int sonarRightMemory[90];
int startAngleMemory[4];
int cumulativeAngleMemory[30];
int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(9600);
  while (!Serial);

  myservo1.attach(4);  // attaches the servo1 on pin 4 to the Servo object
  myservo2.attach(5);  // attaches the servo2 on pin 5 to the Servo object
  myservo3.attach(6);  // attaches the servo3 on pin 6 to the Servo object
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

  for (pos = 90; pos >=0; pos -= 10) { // goes from 90 degrees to 0 degrees per 10 derajat
    // in steps of 1 degree
    myservo1.write(pos);  
    myservo2.write(pos); 
    myservo3.write(pos);              // tell servo to go to position in variable 'pos'
    PrintSerial(pos);
    delay(0.1);                       // waits 15 ms for the servo to reach the position
  }

  for (pos = 0; pos <= 90; pos += 10) { // goes from 0 degrees to 90 degrees per 10 derajat
    myservo1.write(pos); 
    myservo2.write(pos); 
    myservo3.write(pos);              // tell servo to go to position in variable 'pos'
    PrintSerial(pos);
    delay(0.1);                       // waits 15 ms for the servo to reach the position
  }

  address2 = address;
  send = 0;
}

void clearArrays() {
  for (int i = 0; i < 75; i++) {
    sonarFrontMemory[i] = 0;
    sonarLeftMemory[i] = 0;
    sonarRightMemory[i] = 0;
    angleMemory[i] = 0;
  }
  for (int i = 0; i < 4; i++) {
    startAngleMemory[i] = 0;
  }
}

int PrintSerial(int i){
  int sonarFront = sonarModule.kalmanFilter(0);
  int sonarLeft = sonarModule.kalmanFilter(1);
  int sonarRight = sonarModule.kalmanFilter(2);

  sonarFrontMemory[i] = sonarFront;
  sonarLeftMemory[i] = sonarLeft;
  sonarRightMemory[i] = sonarRight;

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
