#include <Arduino.h>
#include "Sonar.h"
#include <Servo.h>

SonarModule sonarModule;

Servo myservo1; 
Servo myservo2; 
Servo myservo3; 

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

  myservo1.attach(4);  
  myservo2.attach(5);  
  myservo3.attach(6);  
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

  for (pos = 90; pos >=0; pos -= 10) { 
    myservo1.write(pos);  
    myservo2.write(pos); 
    myservo3.write(pos);            
    PrintSerial(pos);
    delay(0.1);                     
  }

  for (pos = 0; pos <= 90; pos += 10) { 
    myservo1.write(pos); 
    myservo2.write(pos); 
    myservo3.write(pos);             
    PrintSerial(pos);
    delay(0.1);                      
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
