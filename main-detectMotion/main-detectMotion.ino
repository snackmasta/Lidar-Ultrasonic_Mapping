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
int counter = 1;
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
  int currentAngle;
  int startAngle;
  int stopAngle;

  detectMotions(currentAngle, startAngle, stopAngle); 

  int sonarFront = sonarKalman(0);
  int sonarLeft = sonarKalman(1);
  int sonarRight = sonarKalman(2);

  Serial.print(currentAngle);
  Serial.print(",");
  Serial.print(sonarFront); // front
  Serial.print(",");
  Serial.print(sonarFront); //
  Serial.print(",");
  Serial.print(sonarLeft); // left
  Serial.print(",");
  Serial.print(sonarRight); // right
  Serial.print(",");
  Serial.println(logRecord());

  delay(1);
}

int logRecord(){
  // Check if the pushbutton is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    logNumber++;
    delay(200); // Debounce delay
  }
  return logNumber;
}

int sonarKalman(int i){
  // Kalman filter
  SensorData = sonar[i].ping_cm();
  Xt_update[i] = Xt_prev[i];
  Pt_update[i] = Pt_prev[i] + Q;
  Kt[i] = Pt_update[i] / (Pt_update[i] + R);
  Xt[i] = Xt_update[i] + (Kt[i] * (SensorData - Xt_update[i]));
  Pt[i] = (1 - Kt[i]) * Pt_update[i];

  Xt_prev[i] = Xt[i];
  Pt_prev[i] = Pt[i];

  return (int)Xt[i];
}

int detectMotions(int &currentAngle, int &startAngle, int &stopAngle){
  // counter reset at 10
  if (counter == 10) {
    counter = 1;
  }
  else {
    counter++;
  }

  // Get the current compass angle
  float compassAngle = compass.getCompassAngle();
  // Convert compassAngle to an integer by truncating
  currentAngle = (int)compassAngle;

  // Check if the compass is in motion with tolerance of 10 degrees
  if (abs(currentAngle - startAngle) > 10) {
    startAngle = currentAngle;
    stopAngle = currentAngle;
  }
  else {
    stopAngle = currentAngle;
  }
  
  // print counter
  Serial.print(counter);
  Serial.print(",");
}