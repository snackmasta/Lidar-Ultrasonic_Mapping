#include "compass.h"

const uint8_t compassRxPin = 2;
const uint8_t compassTxPin = 3;
SoftwareSerial compassSerial(compassRxPin, compassTxPin);
GY26_Uart_Compass compass(&compassSerial);

void setupCompass() {
    compassSerial.begin(9600);
    compass.setDeclinationAngle(0.0);
}

float getCompassAngle() {
    float angle = compass.getCompassAngle();
    return angle;
}

