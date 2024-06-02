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
