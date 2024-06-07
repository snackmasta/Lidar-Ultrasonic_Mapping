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

  float compassAngle = getCompassAngle();
  currentAngle = (int)compassAngle;

  // calculate the lastAngle mean
  float mean = 0;
  for (int i = 0; i < 10; i++) {
    mean += lastAngle[i];
  }
  mean = mean / 10;

  int diff = abs(mean - currentAngle);
  
  // Save then capture the startAngle and stopAngle (threshold = 10 degrees)
  if (diff < 5) {
    startAngle = lastAngle[0];
    stopAngle = currentAngle;
  }
  else {
    stopAngle = currentAngle;
  }    
  
  lastAngle[counter++] = currentAngle;

  // Serial.println(logRecord());
  Serial.print("Start Angle: ");
  Serial.print(startAngle);
  Serial.print(", Stop Angle: ");
  Serial.print(stopAngle);
  Serial.print(", Diff: ");
  Serial.print(diff);
  Serial.print(" ");

  // debug last angle array
  for (int i = 0; i < 10; i++) {
    Serial.print(lastAngle[i]);
    Serial.print(",");
  }

  Serial.println();
}
