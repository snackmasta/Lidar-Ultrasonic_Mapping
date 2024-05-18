#include <Wire.h>
#include <GY26Compass.h>

/*
 * This is the compass interface. 
 * The default I2C address is 0x70
 */
GY26_I2C_Compass compass(0x70);

/*
 * Set this to the correct declination angle 
 * for your location
 */
float localDeclinationAngle = 0.0; 

float compassAngle;

void setup(){
    Serial.begin(9600); // Start serial monitor
    Wire.begin();       // Start I2C connectivity
    compass.setDeclinationAngle(localDeclinationAngle);
}

void loop(){
  float compassAngle = compass.getCompassAngle();
  int compassAngleInt = round(compassAngle);
  Serial.print(compassAngleInt);
  Serial.print(",50,100");
  Serial.println();
  delay(500);
}