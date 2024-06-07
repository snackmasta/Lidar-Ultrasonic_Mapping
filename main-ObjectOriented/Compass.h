#ifndef COMPASS_H
#define COMPASS_H

#include <GY26Compass.h>

class CompassModule {
public:
  CompassModule();
  void setDeclinationAngle(float angle);
  float getCompassAngle();
  int detectMotions(int &currentAngle, int &startAngle, int &stopAngle, int &diff);

private:
  GY26_I2C_Compass compass;
  int lastAngle[25];
  int counter;
  int diff;
};

#endif
