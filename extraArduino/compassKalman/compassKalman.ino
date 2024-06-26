#include <SoftwareSerial.h>
#include "TFMini.h"
#include <NewPing.h>
#include <GY26Compass.h>

#define TRIGGER_PIN  12 
#define ECHO_PIN     11  
#define TRIGGER_PIN2  10  // Second sonar sensor trigger pin
#define ECHO_PIN2     9   // Second sonar sensor echo pin
#define TRIGGER_PIN3  8   // Third sonar sensor trigger pin
#define ECHO_PIN3     7   // Third sonar sensor echo pin

#define MAX_DISTANCE 200 
#define BUTTON_PIN   6    // Define the pin for the pushbutton

NewPing sonar1(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE); 
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE); 
GY26_I2C_Compass compass(0x70);
TFMini tfmini;
SoftwareSerial SerialTFMini(4, 5); // The only value that matters here is the first one, 2, Rx

int it = 1; // Number of iterations for averaging distance
float localDeclinationAngle = 0.0; 
float compassAngle;
int dist;
int logNumber = 1; // Initialize logNumber to 1
float SensorData, KalmanFilterData;
float Xt, Xt_update, Xt_prev;
float Pt, Pt_update, Pt_prev;
float Kt, R, Q;

void getTFminiData(int* distance, int* strength) {
  static char i = 0;
  char j = 0;
  int checksum = 0; 
  static int rx[9];
  if(SerialTFMini.available()) {  
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
    } else {
      i++;
    } 
  }  
}

void setup() {  
  pinMode(10, OUTPUT); // Used to trigger
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor
  
  // Step 1: Initialize hardware serial port (serial debug port)
  Serial.begin(9600);
  // wait for serial port to connect. Needed for native USB port only
  while (!Serial);
     
  // Step 2: Initialize the data rate for the SoftwareSerial port
  SerialTFMini.begin(TFMINI_BAUDRATE);

  // Step 3: Initialize the TF Mini sensor
  tfmini.begin(&SerialTFMini);    
  Wire.begin();       // Start I2C connectivity
  compass.setDeclinationAngle(localDeclinationAngle - 6);

  // Kalman filter
  R=10;
  Q=0.5;
  Pt_prev=1;
}

void loop() {
  int l = 0;
  int t = 0;
  int totalDistance1 = 0;
  int totalDistance2 = 0;
  int totalDistance3 = 0;
  int distance = 0;
  int strength = 0;

  getTFminiData(&distance, &strength);
  while(!distance) {
    getTFminiData(&distance, &strength);
    if(distance) {
      totalDistance1 = 0;
      for (t = 0; t < it; t++) {
        unsigned int uS1 = sonar1.ping_cm();
        unsigned int uS2 = sonar2.ping_cm();
        unsigned int uS3 = sonar3.ping_cm();
        totalDistance1 += uS1;
        totalDistance1 += uS2;
        totalDistance1 += uS3;
      }

      int averageDistance1 = totalDistance1 / it;
      int averageDistance2 = totalDistance2 / it;
      int averageDistance3 = totalDistance3 / it;
      float compassAngle = compass.getCompassAngle();
      // Convert compassAngle to an integer by truncating
      int compassAngleInt = (int)compassAngle;

      // kalman filter
      SensorData = compassAngleInt;
      Xt_update = Xt_prev;
      Pt_update = Pt_prev + Q;
      Kt = Pt_update/(Pt_update + R);
      Xt = Xt_update + (Kt * (SensorData - Xt_update));
      Pt = (1-Kt)*Pt_update;
    
      Xt_prev = Xt;
      Pt_prev = Pt;

      KalmanFilterData = Xt;

      // Convert KalmanFilterData to an integer by truncating
      int KalmanFilterDataInt = (int)KalmanFilterData;

      Serial.print(compassAngleInt);
      Serial.print(",");
      Serial.print(KalmanFilterDataInt);
      Serial.print(",");
      Serial.print(averageDistance1);
      Serial.print(",");
      Serial.print(logNumber);
      Serial.print(",");
      Serial.print(KalmanFilterDataInt);
    }
  }

  getTFminiData(&distance, &strength);
  while(!distance) {
    getTFminiData(&distance, &strength);
    if(distance) {
      dist = distance;
    }
  }

  // Check if the pushbutton is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    logNumber++;
    delay(200); // Debounce delay
  }

  delay(10);
}
