#include <SoftwareSerial.h>
#include "TFMini.h"
#include <NewPing.h>
#include <GY26Compass.h>

#define TRIGGER_PIN_1  12  // Sonar 1
#define ECHO_PIN_1     11
#define TRIGGER_PIN_2  10  // Sonar 2
#define ECHO_PIN_2     9
#define TRIGGER_PIN_3   8  // Sonar 3
#define ECHO_PIN_3     7
#define SONAR_NUM 3
#define MAX_DISTANCE 200 
#define BUTTON_PIN   6    // Define the pin for the pushbutton

NewPing sonar[SONAR_NUM] = {   
  NewPing(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_3, ECHO_PIN_3, MAX_DISTANCE)
};

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
  int totalDistance = 0;
  int distance = 0;
  int strength = 0;

  getTFminiData(&distance, &strength);
  while(!distance) {
    getTFminiData(&distance, &strength);
    if(distance) {
      totalDistance = 0;
      int averageDistance = totalDistance / it;
      float compassAngle = compass.getCompassAngle();
      // Convert compassAngle to an integer by truncating
      int compassAngleInt = (int)compassAngle;

      // kalman filter
      SensorData = compassAngle;
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
      Serial.print(distance);
      
      for (uint8_t i = 0; i < SONAR_NUM; i++) {
        Serial.print(",");
        Serial.print(sonar[i].ping_cm());
      }
      
      Serial.print(",");
      Serial.println(logNumber);
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
