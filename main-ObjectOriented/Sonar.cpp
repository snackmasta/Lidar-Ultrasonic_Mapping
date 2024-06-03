#include "Sonar.h"

SonarModule::SonarModule() :
  sonar{
    NewPing(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE),
    NewPing(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE),
    NewPing(TRIGGER_PIN_3, ECHO_PIN_3, MAX_DISTANCE)
  }
{
  // Initialize Kalman filter parameters
  R = 10;
  Q = 0.5;
  for (int i = 0; i < SONAR_NUM; i++) {
    Pt_prev[i] = 1;
  }
}

int SonarModule::ping(int index) {
  return sonar[index].ping_cm();
}

int SonarModule::kalmanFilter(int i){
  // Kalman filter
  float SensorData = ping(i);
  Xt_update[i] = Xt_prev[i];
  Pt_update[i] = Pt_prev[i] + Q;
  Kt[i] = Pt_update[i] / (Pt_update[i] + R);
  Xt[i] = Xt_update[i] + (Kt[i] * (SensorData - Xt_update[i]));
  Pt[i] = (1 - Kt[i]) * Pt_update[i];

  Xt_prev[i] = Xt[i];
  Pt_prev[i] = Pt[i];

  return (int)Xt[i];
}
