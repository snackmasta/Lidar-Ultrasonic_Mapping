#ifndef COMPASS_H
#define COMPASS_H

#include <SoftwareSerial.h>
#include <GY26Compass.h>

extern SoftwareSerial compassSerial;
extern GY26_Uart_Compass compass;

void setupCompass();
float getCompassAngle();

#endif // COMPASS_H

