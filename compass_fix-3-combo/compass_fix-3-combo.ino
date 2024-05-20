#include <SoftwareSerial.h>
#include "TFMini.h"
#include <GY26Compass.h>

// Define pin numbers for SoftwareSerial
const uint8_t lidarRxPin = 4;
const uint8_t lidarTxPin = 5;

// Define pin numbers for compass
const uint8_t compassRxPin = 2;
const uint8_t compassTxPin = 3;

// Initialize SoftwareSerial objects
SoftwareSerial lidarSerial(lidarRxPin, lidarTxPin);
SoftwareSerial compassSerial(compassRxPin, compassTxPin);

// Initialize TFMini and GY26 compass objects
TFMini tfmini;
GY26_Uart_Compass compass(&compassSerial);

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);

  // Initialize SoftwareSerial for lidar and compass
  lidarSerial.begin(TFMINI_BAUDRATE);
  compassSerial.begin(9600);

  // Initialize TFMini and set declination angle for compass
  tfmini.begin(&lidarSerial);
  compass.setDeclinationAngle(0.0);
}

void loop() {
  // Variables to store lidar distance and compass angle
  int distance = 0;
  float angle = 0.0;

  // Get lidar distance data
  getTFminiData(&distance);

  // Get compass angle data
  angle = compass.getCompassAngle();

  // Print angle and distance in the format "<angle>,<distance>"
  Serial.print(angle);
  Serial.print(",");
  Serial.println(distance);

  // Delay for stability
  delay(100);
}

void getTFminiData(int* distance) {
  static char i = 0;
  char j = 0;
  int checksum = 0; 
  static int rx[9];
  
  if(lidarSerial.available()) {  
    rx[i] = lidarSerial.read();
    if(rx[0] != 0x59) {
      i = 0;
    } else if(i == 1 && rx[1] != 0x59) {
      i = 0;
    } else if(i == 8) {
      for(j = 0; j < 8; j++) {
        checksum += rx[j];
      }
      if(rx[8] == (checksum % 256)) {
        *distance = rx[2] + rx[3] * 256;
      }
      i = 0;
    } else {
      i++;
    } 
  }  
}
