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
  int trigger;
  static int counter = 0;
  int memoryCounter = compassModule.detectMotions(currentAngle, startAngle, stopAngle);

  int sonarFront = sonarModule.kalmanFilter(0);
  int sonarLeft = sonarModule.kalmanFilter(1);
  int sonarRight = sonarModule.kalmanFilter(2);
  
  angleMemory[memoryCounter] = currentAngle;
  sonarFrontMemory[memoryCounter] = sonarFront;
  sonarLeftMemory[memoryCounter] = sonarLeft;
  sonarRightMemory[memoryCounter] = sonarRight;
  startAngleMemory[counter] = startAngle; 

  if (counter == 3) {
    counter = 0;
  }
  counter++;

//   Serial.print(startAngleMemory[1]);
//   Serial.print(",");
//   Serial.print(counter);
//   Serial.print(",");
//   Serial.println(startAngleMemory[3]);

  if (abs(startAngleMemory[1] - startAngleMemory[3]) > 30) {
    for (int i = 0; i < 75; i++) {
     Serial.print(angleMemory[i]);
     Serial.print(",");
     Serial.print(sonarFrontMemory[i]);
     Serial.print(",");
     Serial.print(sonarLeftMemory[i]);
     Serial.print(",");
     Serial.print(sonarRightMemory[i]);
     Serial.print(",");
     Serial.println(logRecord());
    }
  }

  delay(1);
}

int logRecord(){
  if (buttonModule.isPressed()) {
    logNumber++;
    delay(200); // Debounce delay
  }
  return logNumber;
}
