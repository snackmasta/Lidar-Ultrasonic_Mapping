#include "Compass.h"
#include <Wire.h>

CompassModule::CompassModule() {
  Wire.begin();       // Start I2C connectivity
}

void CompassModule::setDeclinationAngle(float angle) {
  compass.setDeclinationAngle(angle);
}

float CompassModule::getCompassAngle() {
  return compass.getCompassAngle();
}

int CompassModule::detectMotions(int &currentAngle, int &startAngle, int &stopAngle){
  // counter last 10 angles
  if (counter == 10) {
    counter = 0;
  }

  // another counter up to 75 named memoryCounter
  if (memoryCounter == 75) {
    memoryCounter = 0;
  }
  memoryCounter++;

  float compassAngle = getCompassAngle();
  currentAngle = (int)compassAngle;
  
  // Save then capture the startAngle and stopAngle (threshold = 10 degrees)
  if (abs(lastAngle[0] - lastAngle[9]) > 10) {
    startAngle = lastAngle[0];
    stopAngle = currentAngle;
  }
  else {
    stopAngle = currentAngle;
  }    
  
  lastAngle[counter++] = currentAngle;
  
  int diff = abs(startAngle - stopAngle);
  return memoryCounter;
}
