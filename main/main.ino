#include <SoftwareSerial.h>
#include "TFMini.h"
#include <NewPing.h>
#include <GY26Compass.h>

#define TRIGGER_PIN  12 
#define ECHO_PIN     11  
#define MAX_DISTANCE 200 

int it = 10; // Number of iterations for averaging distance
float localDeclinationAngle = 0.0; 
float compassAngle;
int dist;
const uint8_t softRxPin = 2;
const uint8_t softTxPin = 3;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
GY26_I2C_Compass compass(0x70);
TFMini tfmini;
SoftwareSerial SerialTFMini(4, 5); 
SoftwareSerial compassSerial(softRxPin, softTxPin);

void getTFminiData(int* distance, int* strength) {
  static char i = 0;
  char j = 0;
  int checksum = 0; 
  static int rx[9];
  if(SerialTFMini.available())
  {  
    rx[i] = SerialTFMini.read();
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
        *strength = rx[4] + rx[5] * 256;
      }
      i = 0;
    } else 
    {
      i++;
    } 
  }  
}

void setup() {  
  pinMode(10, OUTPUT); // Used to trigger
  int ledPin = 10;
  
  // Step 1: Initialize hardware serial port (serial debug port)
  Serial.begin(9600);
  // wait for serial port to connect. Needed for native USB port only
  while (!Serial);
     
  // Step 2: Initialize the data rate for the SoftwareSerial port
  SerialTFMini.begin(TFMINI_BAUDRATE);

  // Step 3: Initialize the TF Mini sensor
  tfmini.begin(&SerialTFMini);    
  Wire.begin();       // Start I2C connectivity
  compass.setDeclinationAngle(localDeclinationAngle);
}

void loop() {
  int l = 0;
  int t = 0;
  int totalDistance = 0;
  int distance = 0;
  int strength = 0;

  getTFminiData(&distance, &strength);
  while(!distance) {
    getTFminiData(&distance, &strength);
    if(distance) {

    totalDistance = 0;
    for (t = 0; t < it; t++) {
      unsigned int uS = sonar.ping_cm();
      totalDistance += uS;
      delay(10);
     }
     
      int averageDistance = totalDistance / it;
      float compassAngle = compass.getCompassAngle();
      int compassAngleInt = (int)compassAngle;
      Serial.print(compassAngleInt);
      Serial.print(",");
      Serial.print(averageDistance);
      Serial.print(",");
      Serial.println(distance);
    }
  }
    
  getTFminiData(&distance, &strength);

  while(!distance) {
    getTFminiData(&distance, &strength);
    if(distance) {
      dist = distance;
    }
  }

  delay(100);
}