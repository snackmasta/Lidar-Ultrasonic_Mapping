#include <Servo.h>
#include <SoftwareSerial.h>
#include <NewPing.h>
#include <GY26Compass.h>

// Servo
Servo servo1;
Servo servo2;
Servo servo3;
int pos = 0;

// Sonar and Compass
#define TRIGGER_PIN_1  12
#define ECHO_PIN_1     11
#define TRIGGER_PIN_2  10
#define ECHO_PIN_2     9
#define TRIGGER_PIN_3  8
#define ECHO_PIN_3     7
#define SONAR_NUM 3
#define MAX_DISTANCE 200
#define BUTTON_PIN   6

NewPing sonar[SONAR_NUM] = {
  NewPing(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_3, ECHO_PIN_3, MAX_DISTANCE)
};

GY26_I2C_Compass compass(0x70);

int it = 1;
float localDeclinationAngle = 0.0;
float compassAngle;
int dist;
int logNumber = 1;
float SensorData, lidarFilter;
float Xt[SONAR_NUM], Xt_update[SONAR_NUM], Xt_prev[SONAR_NUM];
float Pt[SONAR_NUM], Pt_update[SONAR_NUM], Pt_prev[SONAR_NUM];
float Kt[SONAR_NUM], R, Q;

void setup() {
  // Servo setup
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);

  // Sonar and Compass setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  while (!Serial);

  Wire.begin();
  compass.setDeclinationAngle(localDeclinationAngle - 6);

  R = 10;
  Q = 0.5;
  for (int i = 0; i < SONAR_NUM; i++) {
    Pt_prev[i] = 1;
  }
}

void loop() {
  // Servo movement
  for (pos = 0; pos <= 180; pos += 1) {
    servo1.write(pos);
    servo2.write(pos);
    servo3.write(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    servo1.write(pos);
    servo2.write(pos);
    servo3.write(pos);
    delay(15);
  }

  // Sonar and Compass processing
  int totalDistance = 0;
  int averageDistance = totalDistance / it;
  float compassAngle = compass.getCompassAngle();
  int compassAngleInt = (int)compassAngle;

  int sonarFiltered[SONAR_NUM];

  for (uint8_t i = 0; i < SONAR_NUM; i++) {
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
  Serial.print(sonarFiltered[1]);
  Serial.print(",");
  Serial.print(sonarFiltered[2]);
  Serial.print(",");
  Serial.println(logNumber);

  if (digitalRead(BUTTON_PIN) == LOW) {
    logNumber++;
    delay(200);
  }

  delay(10);
}
