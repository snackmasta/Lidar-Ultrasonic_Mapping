#include <SoftwareSerial.h>
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

int it = 1; // Number of iterations for averaging distance
float localDeclinationAngle = 0.0; 
float compassAngle;
int dist;
int logNumber = 1; // Initialize logNumber to 1
float SensorData, lidarFilter;
float Xt[SONAR_NUM], Xt_update[SONAR_NUM], Xt_prev[SONAR_NUM];
float Pt[SONAR_NUM], Pt_update[SONAR_NUM], Pt_prev[SONAR_NUM];
float Kt[SONAR_NUM], R, Q;

void setup() {  
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor
  
  // Step 1: Initialize hardware serial port (serial debug port)
  Serial.begin(9600);
  // wait for serial port to connect. Needed for native USB port only
  while (!Serial);

  Wire.begin();       // Start I2C connectivity
  compass.setDeclinationAngle(localDeclinationAngle - 6);

  // Kalman filter
  R = 10;
  Q = 0.5;
  for (int i = 0; i < SONAR_NUM; i++) {
    Pt_prev[i] = 1;
  }
}

void loop() {
  int totalDistance = 0;

  totalDistance = 0;
  int averageDistance = totalDistance / it;
  float compassAngle = compass.getCompassAngle();
  // Convert compassAngle to an integer by truncating
  int compassAngleInt = (int)compassAngle;

  int sonarFiltered[SONAR_NUM];

  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    // Kalman filter
    SensorData = sonar[i].ping_cm();
    Xt_update[i] = Xt_prev[i];
    Pt_update[i] = Pt_prev[i] + Q;
    Kt[i] = Pt_update[i] / (Pt_update[i] + R);
    Xt[i] = Xt_update[i] + (Kt[i] * (SensorData - Xt_update[i]));
    Pt[i] = (1 - Kt[i]) * Pt_update[i];
  
    Xt_prev[i] = Xt[i];
    Pt_prev[i] = Pt[i];

    sonarFiltered[i] = (int)Xt[i];
  }

  Serial.print(compassAngleInt);
  Serial.print(",");
  Serial.print(sonarFiltered[0]);
  Serial.print(",");
  Serial.print(sonarFiltered[0]);
  Serial.print(",");
  Serial.print(sonarFiltered[1]);
  Serial.print(",");
  Serial.print(sonarFiltered[2]);
  Serial.print(",");
  Serial.println(logNumber);
    // Check if the pushbutton is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    logNumber++;
    delay(200); // Debounce delay
  }

  delay(10);
}