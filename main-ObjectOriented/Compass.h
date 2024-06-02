#ifndef COMPASS_H
#define COMPASS_H

#include <GY26Compass.h>

class CompassModule {
public:
  CompassModule();
  void setDeclinationAngle(float angle);
  float getCompassAngle();

private:
  GY26_I2C_Compass compass;
};

#endif
