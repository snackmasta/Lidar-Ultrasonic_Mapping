#include <GY26Compass.h>
#include <SoftwareSerial.h>
const uint8_t softRxPin = 2;
const uint8_t softTxPin = 3;
SoftwareSerial compassSerial(softRxPin, softTxPin);
GY26_Uart_Compass compass(&compassSerial);
float localDeclinationAngle = 0.0; 

float compassTemperature;
float compassAngle;

void setup(){
    Serial.begin(9600); // Start serial monitor
    compassSerial.begin(9600); // Start serial connectivity with compass
    compass.setDeclinationAngle(localDeclinationAngle);
}

void loop(){
  compassTemperature = compass.getTemperatureCelsius();
  compassAngle = compass.getCompassAngle();
  Serial.println("=========================================");
  Serial.print("TEMPERATURE (C): ");Serial.println(compassTemperature);
  Serial.print("COMPASS ANGLE:  ");Serial.println(compassAngle);
  Serial.println("=========================================");
  Serial.println();
  delay(1000);
}
