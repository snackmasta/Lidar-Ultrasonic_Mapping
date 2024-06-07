#include <Arduino.h>
#include "Sonar.h"
#include "Button.h"
#include "Compass.h"

SonarModule sonarModule;
ButtonModule buttonModule;
CompassModule compassModule;

int it = 1; // Number of iterations for averaging distance
float localDeclinationAngle = 0.0; 
float compassAngle;
int logNumber = 1; // Initialize logNumber to 1
int sonarFrontMemory[75];
int sonarLeftMemory[75];
int sonarRightMemory[75];
int angleMemory[75];
int startAngleMemory[4];

void setup() {
  Serial.begin(9600);
  while (!Serial);

  compassModule.setDeclinationAngle(localDeclinationAngle - 6);
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

  compassModule.detectMotions(currentAngle, startAngle, stopAngle, diff);

  int sonarFront = sonarModule.kalmanFilter(0);
  int sonarLeft = sonarModule.kalmanFilter(1);
  int sonarRight = sonarModule.kalmanFilter(2);

  // Normalize angles
  startAngle = normalizeAngle(startAngle);
  stopAngle = normalizeAngle(stopAngle);

  // Record angle correlated with sonar
  address = calculateAddress(startAngle, currentAngle);
  // Serial.println(address);
  angleMemory[address] = currentAngle;
  sonarFrontMemory[address] = sonarFront;
  sonarRightMemory[address] = sonarRight;
  sonarLeftMemory[address] = sonarLeft;

  // if address is larger than 10 and counting down
  address1 = address;
  if (abs(address1 - address2) > 20) {
    send = 1;
  }

  if (send == 1) {

    for (int i = 0; i < 75; i++) {
      Serial.print(angleMemory[i]);
      Serial.print(",");
      Serial.print(sonarFrontMemory[i]);
      Serial.print(",");
      Serial.print(sonarFrontMemory[i]);
      Serial.print(",");
      Serial.print(sonarLeftMemory[i]);
      Serial.print(",");
      Serial.print(sonarRightMemory[i]);
      Serial.print(",");
      Serial.println(logRecord());
    }

    for (int ite = 0; ite < 75; ite++) {
      sonarFrontMemory[ite] = 0;
      sonarRightMemory[ite] = 0;
      sonarLeftMemory[ite] = 0;
    }
  }

  address2 = address;
  send = 0;
}

int logRecord() {
  if (buttonModule.isPressed()) {
    logNumber++;
    delay(200); // Debounce delay
  }
  return logNumber;
}

int memoryAddress(bool reset) {
  static int address = 0;
  if (reset) {
    address = 0;
  }
  return address + 1;
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

int normalizeAngle(int angle) {
  while (angle < 0) {
    angle += 360;
  }
  while (angle >= 360) {
    angle -= 360;
  }
  return angle;
}

int calculateAddress(int startAngle, int currentAngle) {
  int diff = abs(currentAngle - startAngle);
  if (diff < 0) {
    diff += 360;
  }
  return diff % 75; // Ensure the address is within the array bounds
}
