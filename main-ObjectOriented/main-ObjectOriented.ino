#include <Arduino.h>
#include "Sonar.h"
#include "Button.h"
#include "Compass.h"

SonarModule sonarModule;
ButtonModule buttonModule;
CompassModule compassModule;

int it = 1; // Number of iterations for averaging distance
float localDeclinationAngle = 0.0; 
float compassAngle;
int logNumber = 1; // Initialize logNumber to 1
int counter = 1;
float SensorData, lidarFilter;
float Xt[SONAR_NUM], Xt_update[SONAR_NUM], Xt_prev[SONAR_NUM];
float Pt[SONAR_NUM], Pt_update[SONAR_NUM], Pt_prev[SONAR_NUM];
float Kt[SONAR_NUM], R, Q;
int lastAngle[25]; // Array to store the last 10 angles

void setup() {
  Serial.begin(9600);
  while (!Serial);

  compassModule.setDeclinationAngle(localDeclinationAngle - 6);

  // Kalman filter
  R = 10;
  Q = 0.5;
  for (int i = 0; i < SONAR_NUM; i++) {
    Pt_prev[i] = 1;
  }
}

void loop() {
  int currentAngle;
  int startAngle;
  int stopAngle;

  detectMotions(currentAngle, startAngle, stopAngle); 

  int sonarFront = sonarKalman(0);
  int sonarLeft = sonarKalman(1);
  int sonarRight = sonarKalman(2);

  Serial.println(logRecord());

  delay(1);
}

int logRecord(){
  if (buttonModule.isPressed()) {
    logNumber++;
    delay(200); // Debounce delay
  }
  return logNumber;
}

int sonarKalman(int i){
  SensorData = sonarModule.ping(i);
  Xt_update[i] = Xt_prev[i];
  Pt_update[i] = Pt_prev[i] + Q;
  Kt[i] = Pt_update[i] / (Pt_update[i] + R);
  Xt[i] = Xt_update[i] + (Kt[i] * (SensorData - Xt_update[i]));
  Pt[i] = (1 - Kt[i]) * Pt_update[i];

  Xt_prev[i] = Xt[i];
  Pt_prev[i] = Pt[i];

  return (int)Xt[i];
}

void detectMotions(int &currentAngle, int &startAngle, int &stopAngle){
  if (counter == 10) {
    counter = 0;
  }

  float compassAngle = compassModule.getCompassAngle();
  currentAngle = (int)compassAngle;

  if (abs(lastAngle[0] - lastAngle[9]) > 10) {
    startAngle = lastAngle[0];
    stopAngle = currentAngle;
  }
  else {
    stopAngle = currentAngle;
  }    
  
  lastAngle[counter++] = currentAngle;

  for (int i = 0; i < 10; i++) {
    Serial.print(lastAngle[i]);
    Serial.print(",");
  }
  Serial.print("startAngle: ");
  Serial.print(startAngle);
  Serial.print(",");
  Serial.print("stopAngle: ");
  Serial.print(stopAngle);
  Serial.print(",");
}
